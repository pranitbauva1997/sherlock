#include <IRremote.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_HMC5883_U.h>

// Pin for IR LED receiver
#define IRpin 11

// Pins for motor
#define ENABLE_MOTOR_L 1
#define ENABLE_MOTOR_R 2
#define POSITIVE_MOTOR_L 3
#define NEGATIVE_MOTOR_L 4
#define POSITIVE_MOTOR_R 5
#define NEGATIVE_MOTOR_R 6

// Define other "constants"
#define MAX_SPEED 150
#define MAGNETIC_DECLINATION 0.575959

// Declaring the sensors
Adafruit_HMC5883_Unified mag = Adafruit_HMC5883_Unified(12345);
sensors_event_t event;
IRrecv irrecv(IRpin);
decode_results results;


// Global Variables to be used instead of passing as parameters
float dst_direction, current_direction;
int start_poi = -1, end_poi = -1;
int current_poi, dst_poi;
String received;

// Method declarations
void getPath();
void move_();
void getmessage();
int decode_hex(String received);

void move_(int l, int r){
  if(l > 0 && r > 0){
    analogWrite(ENABLE_MOTOR_L, l);
    analogWrite(ENABLE_MOTOR_R, r);

    digitalWrite(POSITIVE_MOTOR_L, HIGH);
    digitalWrite(NEGATIVE_MOTOR_L, LOW);

    digitalWrite(POSITIVE_MOTOR_R, HIGH);
    digitalWrite(NEGATIVE_MOTOR_R, LOW);
  }
  else if(l > 0 && r < 0){
    analogWrite(ENABLE_MOTOR_L, l);
    analogWrite(ENABLE_MOTOR_R, -r);

    digitalWrite(POSITIVE_MOTOR_L, HIGH);
    digitalWrite(NEGATIVE_MOTOR_L, LOW);

    digitalWrite(POSITIVE_MOTOR_R, LOW);
    digitalWrite(NEGATIVE_MOTOR_R, HIGH);
  }
  else if(l < 0 && r > 0){
    analogWrite(ENABLE_MOTOR_L, -l);
    analogWrite(ENABLE_MOTOR_R, r);

    digitalWrite(POSITIVE_MOTOR_L, LOW);
    digitalWrite(NEGATIVE_MOTOR_L, HIGH);

    digitalWrite(POSITIVE_MOTOR_R, HIGH);
    digitalWrite(NEGATIVE_MOTOR_R, LOW);
  }
  else{
    analogWrite(ENABLE_MOTOR_L, -l);
    analogWrite(ENABLE_MOTOR_R, -r);

    digitalWrite(POSITIVE_MOTOR_L, LOW);
    digitalWrite(NEGATIVE_MOTOR_L, HIGH);

    digitalWrite(POSITIVE_MOTOR_R, LOW);
    digitalWrite(NEGATIVE_MOTOR_R, HIGH);
  }
}

void getPath(){
  // This part needs to be physically tested to know how the degree's are
  // calibrated
  
  if(dst_direction - current_direction > 0){
    if(dst_direction - current_direction > 100){
      move(+MAX_SPEED, -MAX_SPEED);
      delay(200);
    }
    if(dst_direction - current_direction < 50){
      move_(+MAX_SPEED, 0);
      delay(200);
    }
    if(dst_direction - current_direction < 20){
      move_(+MAX_SPEED/2, +MAX_SPEED/5);
      delay(200);
    }
    if(dst_direction - current_direction < 5){
      move_(+MAX_SPEED, +MAX_SPEED)
      delay(200);
    }
  }
  if(dst_direction - current_direction < 0){
    if(dst_direction - current_direction > 100){
      move_(-MAX_SPEED, +MAX_SPEED);
      delay(200);
    }
    if(dst_direction - current_direction < 50){
      move_(0, +MAX_SPEED);
      delay(200);
    }
    if(dst_direction - current_direction < 20){
      move_(+MAX_SPEED/5, +MAX_SPEED/2);
      delay(200);
    }
    if(dst_direction - current_direction < 5){
      move_(+MAX_SPEED, +MAX_SPEED);
      delay(200);
    }
  }
}

void getmessage(){
  irrecv.resume();
  received = String(results.value, HEX);
  if(decode_hex(received)){
    if(results.value % 2 == 1){
      irrecv.resume();
      received = String(results.value, HEX);
      if(decode_hex(received)){
        current_poi = results.value;
        irrecv.resume();
        received = String(results.value, HEX);
        if(decode_hex(received)){
          dst_poi = results.value;
          irrecv.resume();
          received = String(results.value, HEX);
          if(decode_hex(received)){
            dst_direction = results.value;
            irrecv.resume();
            received = String(results.value, HEX);
            if(decode_hex(received)){
              if(results.value != 500){
                move_(0, 0);
              }
            }
          }
        }
      }
    }
  }
  else{
    irrecv.resume();
    received = String(results.value, HEX);
    if(decode_hex(received)){
      start_poi = results.value;
      irrecv.resume();
      received = String(results.value, HEX);
      if(decode_hex(received)){
        end_poi = results.value;
        irrecv.resume();
        received = String(results.value, HEX);
        if(decode_hex(received)){
          if(results.value != 500){
            move_(0, 0);
          }
          irrecv.resume();
          received = String(results.value, HEX);
          if(decode_hex(received)){
            if(results.value == 400){
              irrecv.resume();
              received = String(results.value, HEX);
              if(decode_hex(received)){
                if(results.value % 2 == 0){
                  irrecv.resume();
                  received = String(results.value, HEX);
                  if(decode_hex(received)){
                    current_poi = results.value;
                    irrecv.resume();
                    received = String(results.value, HEX);
                    if(decode_hex(received)){
                      dst_poi = results.value;
                      irrecv.resume();
                      received = String(results.value, HEX);
                      if(decode_hex(received)){
                        dst_direction = results.value;
                        irrecv.resume();
                        irrecv.resume();
                        received = String(results.value, HEX);
                        if(results.value != 500){
                          move_(0, 0);
                        }
                      }
                    }
                  }
                }
              }
            }
          }
        }
      }
    }
  }
}

void setup(){
  Serial.begin(9600);
  
  // Setup of magnetometer
  if(!mag.begin()){
    Serial.println("Oops, no HMC5883L detected ... Check your wiring");
    while(1);
  }

  // Setup of IR Receiver
  irrecv.enableIRIn();

  // Setup the motors
  pinMode(ENABLE_MOTOR_L, OUTPUT);
  pinMode(ENABLE_MOTOR_R, OUTPUT);
  pinMode(POSITIVE_MOTOR_R, OUTPUT);
  pinMode(NEGATIVE_MOTOR_R, OUTPUT);
  pinMode(POSITIVE_MOTOR_R, OUTPUT);
  pinMode(NEGATIVE_MOTOR_L, OUTPUT);
}

void loop(){

  // Get and decode the signal from IR receiver
  if(decode_hex(received)){
    received = String(results.value, HEX);
    if(decode_hex(received) == 400){
      getmessage();
    }
  }


  // Get the heading from magnetometer
  mag.getEvent(&event);
  current_direction = atan2(event.magnetic.y, event.magnetic.x);
  // Correct the error supplied by magenotometer because of "magnetic declication"
  current_direction += MAGNETIC_DECLINATION;
  
  if(current_direction < 0);
    current_direction += 2*PI;

  if(current_direction > 2*PI)
    current_direction -= 2*PI;

  current_direction = current_direction * 180/M_PI;

  // The angle should be between (-180, 180]
  if(current_direction > 180){
    current_direction = current_direction - 360;
  }


  // Getting the path to move the bot
  getPath();
}

int decode_hex(String input)
{
    int zeros = 8-input.length();
    for(int i=0; i<zeros; i++) {
        input = "0" + input;
    }
    String hex_data = input.substring(0,2) + input.substring(4,6);
    const char * c = hex_data.c_str();
    int output = (int) strtol(c, NULL, 16);
    return output;
}
