#include <IRremote.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_HMC5883_U.h>

// Pin for IR LED receiver
#define IRpin 11

// Pin for motor PWM
#define MOTOR_L 1
#define MOTOR_R 2

// Declaring the sensors
Adafruit_HMC5883_Unified mag = Adafruit_HMC5883_Unified(12345);
sensors_event_t event;
IRrecv irrecv(IRpin);
decode_results results;


// Global Variables to be used instead of passing as parameters
float dst_direction, current_direction;
int start_poi = -1, end_poi = 1;
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
  
}

void getmessage(){
  irrecv.resume();
  if(results.value % 2 == 1){
    irrecv.resume();
    current_poi = results.value;
    irrecv.resume();
    dst_poi = results.value;
    irrecv.resume();
    dst_direction = results.value;
    irrecv.resume();
    if(results.value != 500){
      move_(0, 0);
    }
  }
  else{
    irrecv.resume();
    start_poi = results.value;
    irrecv.resume();
    end_poi = results.value;
    irrecv.resume();
    if(results.value != 500){
      move_(0, 0);
    }
    irrecv.resume();
    current_poi = results.value;
    irrecv.resume();
    dst_poi = results.value;
    irrecv.resume();
    dst_direction = results.value;
    irrecv.resume();
    irrecv.resume();
    if(results.value != 500){
      move_(0, 0);
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


  // Getting the path to move the bot
  getPath();
}

