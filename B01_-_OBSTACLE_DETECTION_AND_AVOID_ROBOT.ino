#include <Servo.h>

const int trigPinFront = 9;
const int echoPinFront = 8;

const int trigPinSide = 12;
const int echoPinSide = 11;

const int ENA = 5;
const int IN1 = 2;
const int IN2 = 3;
const int ENB = 6;
const int IN3 = 4;
const int IN4 = 7;

Servo taskServo;
const int servoPin = 10;

int frontDistance = 0;
int sideDistance = 0;
const int OBSTACLE_DISTANCE = 20; // cm

void setup() 
{
  
  Serial.begin(9600);
  Serial.println("Precision Farming Robot Initialized");

  pinMode(trigPinFront, OUTPUT);
  pinMode(echoPinFront, INPUT);
  pinMode(trigPinSide, OUTPUT);
  pinMode(echoPinSide, INPUT);

  pinMode(ENA, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(ENB, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  
  taskServo.attach(servoPin);
  taskServo.write(90); 

  delay(1000); 
}

void loop() {
  frontDistance = readUltrasonic(trigPinFront, echoPinFront);
  sideDistance = readUltrasonic(trigPinSide, echoPinSide);


  Serial.print("Front: ");
  Serial.print(frontDistance);
  Serial.print(" cm\tSide: ");
  Serial.print(sideDistance);
  Serial.println(" cm");

  if (frontDistance < OBSTACLE_DISTANCE) 
  {
    Serial.println("Obstacle Detected! Turning Right...");
    stopMotors();
    delay(500);
    turnRight();
    delay(700);
    stopMotors();
  }
  
  else if (sideDistance > 25) 
  {
    Serial.println("Too far from row - Turning Left");
    turnLeft();
    delay(300);
    moveForward();
  }
  else if (sideDistance < 10)
  {
    Serial.println("Too close to row - Turning Right");
    turnRight();
    delay(300);
    moveForward();
  }
  else 
  {
    Serial.println("Path Clear - Moving Forward");
    moveForward();
  }

  Serial.println("Activating Servo - Spraying...");
  taskServo.write(0);
  delay(300);
  taskServo.write(90);
  delay(500);  
}


int readUltrasonic(int trigPin, int echoPin)
{
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  long duration = pulseIn(echoPin, HIGH);
  int distance = duration * 0.034 / 2;
  return distance;
}
void moveForward()
{
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);

  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);

  analogWrite(ENA, 150);
  analogWrite(ENB, 150);
}

void stopMotors() 
{
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);

  analogWrite(ENA, 0);
  analogWrite(ENB, 0);
}

void turnLeft()
{
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);

  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);

  analogWrite(ENA, 150);
  analogWrite(ENB, 150);
}

void turnRight()
{
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);

  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);

  analogWrite(ENA, 150);
  analogWrite(ENB, 150);
}
