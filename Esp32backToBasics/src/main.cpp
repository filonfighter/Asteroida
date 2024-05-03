#include <Arduino.h>

// Motor Control Pins
int motorOxygenCW_EN = 39; // Motor Oxygen Enable Pin
int motorOxygenCCW_EN = 36; // Motor Oxygen Enable Pin
int motorOxygenSpeed = 34; // Motor Oxygen Speed input (must be PWM)

// Motor Open/Close time in miliseconds
unsigned long motorOxygenOpeningTime = 5000; // 5 seconds
unsigned long motorOxygenClosingTime = 5000; // 5 seconds

// Motor closing speed
int motorSpeedClose = 200; // 200/255

// Motor opening speed
int motorSpeedOpen = 255; // 255/255

// Forward declarations
void MotorOxygenOpen(int speed);
void MotorOxygenClose(int speed);
void MotorOxygenStop();

// Pin Setup
void setup() {
  pinMode(motorOxygenCW_EN, OUTPUT);
  pinMode(motorOxygenCCW_EN, OUTPUT);
  pinMode(motorOxygenSpeed, OUTPUT);
}

// Main Loop
void loop() {
  // Open valve
  MotorOxygenOpen(motorSpeedOpen);
  delay(motorOxygenOpeningTime);
  MotorOxygenStop();

  // Close valve
  MotorOxygenClose(motorSpeedClose);
  delay(motorOxygenClosingTime);
  MotorOxygenStop();
}

// Motor Control Functions
void MotorOxygenOpen(int speed) {
  analogWrite(motorOxygenSpeed, speed); // Set speed
  digitalWrite(motorOxygenCW_EN, HIGH);
  digitalWrite(motorOxygenCCW_EN, LOW);
}

void MotorOxygenClose(int speed) {
  analogWrite(motorOxygenSpeed, speed); // Set speed
  digitalWrite(motorOxygenCW_EN, LOW);
  digitalWrite(motorOxygenCCW_EN, HIGH);
}

void MotorOxygenStop() {
  analogWrite(motorOxygenSpeed, 0); // Set speed to 0%
  digitalWrite(motorOxygenCW_EN, LOW);
  digitalWrite(motorOxygenCCW_EN, LOW);
}