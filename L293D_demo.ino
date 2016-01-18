#define ENABLE_MOTOR_L 1
#define ENABLE_MOTOR_R 2

#define POSITIVE_MOTOR_L 3
#define NEGATIVE_MOTOR_L 4

#define POSITIVE_MOTOR_R 6
#define NEGATIVE_MOTOR_R 7

void setup(){
  pinMode(ENABLE_MOTOR_L, OUTPUT);
  pinMode(ENABLE_MOTOR_R, OUTPUT);
  pinMode(POSITIVE_MOTOR_L, OUTPUT);
  pinMode(NEGATIVE_MOTOR_L, OUTPUT);
  pinMode(POSITIVE_MOTOR_R, OUTPUT);
  pinMode(NEGATIVE_MOTOR_L, OUTPUT);
}

void loop(){
  forward();
  delay(500);
  backward();
  delay(500);
  right();
  delay(500);
  left();
  delay(500);
}

void right(){
  analogWrite(ENABLE_MOTOR_L, 150);
  analogWrite(ENABLE_MOTOR_R, 150);

  digitalWrite(POSITIVE_MOTOR_L, HIGH);
  digitalWrite(NEGATIVE_MOTOR_L, LOW);

  digitalWrite(POSITIVE_MOTOR_R, LOW);
  digitalWrite(NEGATIVE_MOTOR_R, HIGH);
  
}

void left(){
  analogWrite(ENABLE_MOTOR_L, 150);
  analogWrite(ENABLE_MOTOR_R, 150);

  digitalWrite(POSITIVE_MOTOR_L, LOW);
  digitalWrite(NEGATIVE_MOTOR_L, HIGH);

  digitalWrite(POSITIVE_MOTOR_R, HIGH);
  digitalWrite(NEGATIVE_MOTOR_R, LOW);
}

void forward(){
  analogWrite(ENABLE_MOTOR_L, 150);
  analogWrite(ENABLE_MOTOR_R, 150);

  digitalWrite(POSITIVE_MOTOR_L, HIGH);
  digitalWrite(NEGATIVE_MOTOR_L, LOW);

  digitalWrite(POSITIVE_MOTOR_R, HIGH);
  digitalWrite(NEGATIVE_MOTOR_R, LOW);
}

void backward(){
  analogWrite(ENABLE_MOTOR_L, 150);
  analogWrite(ENABLE_MOTOR_R, 150);

  digitalWrite(POSITIVE_MOTOR_L, LOW);
  digitalWrite(NEGATIVE_MOTOR_L, HIGH);

  digitalWrite(POSITIVE_MOTOR_R, LOW);
  digitalWrite(NEGATIVE_MOTOR_R, HIGH);
}

