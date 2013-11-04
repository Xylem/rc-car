#define MINIMAL_DISTANCE 4000
#define SWITCH_BACK_DISTANCE 6000
#define TURN_DIFFERENCE 1.05f

#include "enums.h"

void turn(Direction dir) {
  switch (dir) {
    case RIGHT:
      digitalWrite(LEFT, LOW);
      digitalWrite(RIGHT, HIGH);
      break;
    case LEFT:
      digitalWrite(RIGHT, LOW);
      digitalWrite(LEFT, HIGH);
      break;
    default:
      digitalWrite(LEFT, LOW);
      digitalWrite(RIGHT, LOW);
  }
}

void drive(Accel accel) {
  switch (accel) {
  case FORWARD:
    digitalWrite(BACK, LOW);
    digitalWrite(FORWARD, HIGH);
    break;
  case BACK:
    digitalWrite(FORWARD, LOW);
    digitalWrite(BACK, HIGH);
    break;
  default:
    bool forward = digitalRead(FORWARD) == HIGH;
    bool backward = digitalRead(BACK) == HIGH;
    
    digitalWrite(BACK, LOW);
    digitalWrite(FORWARD, LOW);
    
    if (forward) {
      digitalWrite(BACK, HIGH);
      delay(70);
      digitalWrite(BACK, LOW);
    } else if (backward) {
      digitalWrite(FORWARD, HIGH);
      delay(50);
      digitalWrite(FORWARD, LOW);
    }
  }
}

int measureDistance(Distance triggerPin, Distance echoPin) {
  digitalWrite(triggerPin, HIGH);
  delay(5);
  digitalWrite(triggerPin, LOW);

  return pulseIn(echoPin, HIGH);  
}

void setup() {
  // Output initialization

  pinMode(RIGHT, OUTPUT);
  pinMode(LEFT, OUTPUT);
  pinMode(FORWARD, OUTPUT);
  pinMode(BACK, OUTPUT);

  pinMode(TRIGGER_LEFT, OUTPUT);
  pinMode(TRIGGER_RIGHT, OUTPUT);
  
  // Test sequence

  turn(RIGHT);
  delay(1000);

  turn(LEFT);
  delay(1000);

  turn(STRAIGHT);
  drive(BACK);
  delay(1000);

  drive(FORWARD);
  delay(1000);

  drive(STOP);
}

bool minimalReached = false;

void loop() {
  int leftDistance = measureDistance(TRIGGER_LEFT, ECHO_LEFT);
  
  delay(20);
  
  int rightDistance = measureDistance(TRIGGER_RIGHT, ECHO_RIGHT);
  
  if (minimalReached) {
      if (rightDistance > SWITCH_BACK_DISTANCE && leftDistance > SWITCH_BACK_DISTANCE) {
          minimalReached = false;
      }
  } else {
    if (leftDistance < MINIMAL_DISTANCE || rightDistance < MINIMAL_DISTANCE) {
        drive(BACK);
        
        if (leftDistance < rightDistance) {
            turn(LEFT); 
        } else {
            turn(RIGHT); 
        }
        
        minimalReached = true;
    } else {
        drive(FORWARD);
        
        if (leftDistance < rightDistance && rightDistance / (1.0f * leftDistance) > TURN_DIFFERENCE) {
            turn(RIGHT);
        } else if (rightDistance < leftDistance && leftDistance / (1.0f * rightDistance) > TURN_DIFFERENCE) {
            turn(LEFT);   
        } else {
            turn(STRAIGHT);  
        }
    }
  }
  delay(20);
}
