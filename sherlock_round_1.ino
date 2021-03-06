#include <IRremote.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_HMC5883_U.h>

// Pin for IR LED receiver
#define IRpin 11

// Pins for motor
#define ENABLE_MOTOR_L 6
#define ENABLE_MOTOR_R 7
#define POSITIVE_MOTOR_L 24
#define NEGATIVE_MOTOR_L 25
#define POSITIVE_MOTOR_R 28
#define NEGATIVE_MOTOR_R 29

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
int start_poi, end_poi;
int current_poi, dst_poi;
String received;
int message[12];

// Method declarations
void getPath();
void move_();
void getmessage();
int decode_hex(String received);
int store_message();
void print_arr();

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
  else if(l < 0 && r < 0){
    analogWrite(ENABLE_MOTOR_L, -l);
    analogWrite(ENABLE_MOTOR_R, -r);

    digitalWrite(POSITIVE_MOTOR_L, LOW);
    digitalWrite(NEGATIVE_MOTOR_L, HIGH);

    digitalWrite(POSITIVE_MOTOR_R, LOW);
    digitalWrite(NEGATIVE_MOTOR_R, HIGH);
  }
  else if(l > 0 && r == 0){
    analogWrite(ENABLE_MOTOR_L, l);
    analogWrite(ENABLE_MOTOR_R, 0);

    digitalWrite(POSITIVE_MOTOR_L, HIGH);
    digitalWrite(NEGATIVE_MOTOR_L, LOW);

    digitalWrite(POSITIVE_MOTOR_R, LOW);
    digitalWrite(NEGATIVE_MOTOR_R, LOW);
  }
  else if(l < 0 && r == 0){
    analogWrite(ENABLE_MOTOR_L, -l);
    analogWrite(ENABLE_MOTOR_R, 0);

    digitalWrite(POSITIVE_MOTOR_L, LOW);
    digitalWrite(NEGATIVE_MOTOR_L, HIGH);

    digitalWrite(POSITIVE_MOTOR_R, LOW);
    digitalWrite(NEGATIVE_MOTOR_R, LOW);
  }
  else if(l == 0 && r > 0){
    analogWrite(ENABLE_MOTOR_L, 0);
    analogWrite(ENABLE_MOTOR_R, r);

    digitalWrite(POSITIVE_MOTOR_L, LOW);
    digitalWrite(NEGATIVE_MOTOR_L, LOW);

    digitalWrite(POSITIVE_MOTOR_R, HIGH);
    digitalWrite(NEGATIVE_MOTOR_R, LOW);
  }
  else if(l == 0 && r < 0){
    analogWrite(ENABLE_MOTOR_L, 0);
    analogWrite(ENABLE_MOTOR_R, -r);

    digitalWrite(POSITIVE_MOTOR_L, LOW);
    digitalWrite(NEGATIVE_MOTOR_L, LOW);

    digitalWrite(POSITIVE_MOTOR_R, LOW);
    digitalWrite(NEGATIVE_MOTOR_R, HIGH);
  }
  else{
    analogWrite(ENABLE_MOTOR_L, 0);
    analogWrite(ENABLE_MOTOR_R, 0);

    digitalWrite(POSITIVE_MOTOR_L, LOW);
    digitalWrite(NEGATIVE_MOTOR_L, LOW);

    digitalWrite(POSITIVE_MOTOR_R, LOW);
    digitalWrite(NEGATIVE_MOTOR_R, LOW);
  }
}

void getPath(){
  // This part needs to be physically tested to know how the degree's are
  // calibrated

  if(dst_direction - current_direction > 0){
    if(dst_direction - current_direction > 100){
      move_(+MAX_SPEED, -MAX_SPEED);
      delay(500);
    }
    if(dst_direction - current_direction < 50){
      move_(+MAX_SPEED, 0);
      delay(500);
    }
    if(dst_direction - current_direction < 20){
      move_(+MAX_SPEED/2, +MAX_SPEED/5);
      delay(500);
    }
    if(dst_direction - current_direction < 5){
      move_(+MAX_SPEED, +MAX_SPEED);
      delay(500);
    }
  }
  if(dst_direction - current_direction < 0){
    if(dst_direction - current_direction < -100){
      move_(-MAX_SPEED, +MAX_SPEED);
      delay(500);
    }
    if(dst_direction - current_direction > -50){
      move_(0, +MAX_SPEED);
      delay(500);
    }
    if(dst_direction - current_direction > -20){
      move_(+MAX_SPEED/5, +MAX_SPEED/2);
      delay(500);
    }
    if(dst_direction - current_direction > -5){
      move_(+MAX_SPEED, +MAX_SPEED);
      delay(500);
    }
  }
}

int store_message(){
  message[0] = 400;
  int i;
  for(i = 1; i < 12; i++){
    irrecv.resume();
    if(irrecv.decode(&results)){
      message[i] = results.value;
    }
    else
      return -1;

    return 0;
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
  if(irrecv.decode(&results)){
    received = String(results.value, HEX);
    Serial.println(received);
    if(decode_hex(received) == 400)
      if(store_message() != 0)
        loop();
  }

  print_arr();

  // Get the heading from magnetometer
  mag.getEvent(&event);
  current_direction = atan2(event.magnetic.y, event.magnetic.x);
  // Correct the error supplied by magnetometer because of "magnetic declination"
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

void print_arr(){
  int i;
  for(i = 0; i < 12; i++)
    Serial.println(decode_hex(String(message[i], HEX)));
}

