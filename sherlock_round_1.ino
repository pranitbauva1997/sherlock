#include <IRremote.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_HMC5883_U.h>

// Pin for IR LED receiver
#define IRpin 11

// Pin for motor PWM
#define MOTOR_L 1
#define MOTOR_R 2
#define MAX_SPEED 150

// Declaring the sensors
Adafruit_HMC5883_Unified mag = Adafruit_HMC5883_Unified(12345);
sensors_event_t event;
IRrecv irrecv(IRpin);
decode_results results;


// Global Variables to be used instead of passing as parameters
float dst_direction, current_direction;
int start_poi = -1, end_poi = -1;
int current_poi, dst_poi; 

// Method declarations
void getPath();
void move_();
void getmessage();

void move_(int l, int r){
  analogWrite(MOTOR_L, l);
  analogWrite(MOTOR_R, r);
}

void getPath(){
  // This part needs to be physically tested to know how the degree's are
  // calibrated
  
  if(dst_direction - current_direction > 0){
    if(dst_direction - current_direction > 100){
      move_(MOTOR_L, +MAX);
      move_(MOTOR_R, -MAX);
      delay(200);
    }
    if(dst_direction - current_direction < 50){
      move_(MOTOR_L, +MAX);
      move_(MOTOR_R, 0);
      delay(200);
    }
    if(dst_direction - current_direction < 20){
      move_(MOTOR_L, +MAX/2);
      move_(MOTOR_R, +MAX/5);
      delay(200);
    }
    if(dst_direction - current_direction < 5){
      move_(MOTOR_L, +MAX);
      move_(MOTOR_R, +MAX);
      delay(200);
    }
  }
  if(dst_direction - current_direction < 0){
    if(dst_direction - current_direction > 100){
      move_(MOTOR_R, +MAX);
      move_(MOTOR_L, -MAX);
      delay(200);
    }
    if(dst_direction - current_direction < 50){
      move_(MOTOR_R, +MAX);
      move_(MOTOR_L, 0);
      delay(200);
    }
    if(dst_direction - current_direction < 20){
      move_(MOTOR_R, +MAX/2);
      move_(MOTOR_L, +MAX/5);
      delay(200);
    }
    if(dst_direction - current_direction < 5){
      move_(MOTOR_R, +MAX);
      move_(MOTOR_L, +MAX);
      delay(200);
    }
  }
}

void getmessage(){
  irrecv.resume();
  if(irrecv.decode(&results)){
    if(results.value % 2 == 1){
      irrecv.resume();
      if(irrecv.decode(&results)){
        current_poi = results.value;
        irrecv.resume();
        if(irrecv.decode(&results)){
          dst_poi = results.value;
          irrecv.resume();
          if(irrecv.decode(&results)){
            dst_direction = results.value;
            irrecv.resume();
            if(irrecv.decode(&results)){
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
    if(irrecv.decode(&results)){
      start_poi = results.value;
      irrecv.resume();
      if(irrecv.decode(&results)){
        end_poi = results.value;
        irrecv.resume();
        if(irrecv.decode(&results)){
          if(results.value != 500){
            move_(0, 0);
          }
          irrecv.resume();
          if(irrecv.decode(&results)){
            if(results.value % 2 == 0){
              irrecv.resume();
              if(irrecv.decode(&results)){
                current_poi = results.value;
                irrecv.resume();
                if(irrecv.decode(&results)){
                  dst_poi = results.value;
                  irrecv.resume();
                  if(irrecv.decode(&results)){
                    dst_direction = results.value;
                    irrecv.resume();
                    irrecv.resume();
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

void setup(){
  Serial.begin(9600);
  
  // Setup of magnetometer
  if(!mag.begin()){
    Serial.println("Oops, no HMC5883L detected ... Check your wiring");
    while(1);
  }
  pinMode(MOTOR_L, OUTPUT);
  pinMode(MOTOR_R, OUTPUT);
  
}

void loop(){

  // Get and decode the signal from IR receiver
  if(irrecv.decode(&results)){
    Serial.println(results.value, DEC);
    if(results.value == 400){
      getmessage();
    }
  }


  // Get the heading from magnetometer
  mag.getEvent(&event);
  current_direction = atan2(event.magnetic.y, event.magnetic.x) + 0.22;
  
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
