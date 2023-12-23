#include <Arduino.h>

// Following code works so when the button is pressed the start begins opening Oxygen and Fuel valves
// After specified time the valves close and the test is over
// If the button is pressed during the test, the valves close and the test is over

// Motor Control Pins
int motorOxygenEN = 34; // Motor Oxygen Enable Pin (must be PWM)
int motorOxygenIN1 = 32; // Motor Oxygen Input 1 Pin
int motorOxygenIN2 = 33; // Motor Oxygen Input 2 Pin

int motorFuelEN = 35; // Motor Fuel Enable Pin (must be PWM)
int motorFuelIN1 = 25; // Motor Fuel Input 1 Pin
int motorFuelIN2 = 26; // Motor Fuel Input 2 Pin

// Button Pins
int buttonStart = 27; // Start Button Pin

// Test duration in miliseconds
unsigned long waitTime = 5000; // 5 second

// Motor Open/Close time in miliseconds
unsigned long motorTime = 1000; // 1 second

// Motor retrival speed
int motorSpeedRetrive = 200; // 200/255

// Motor opening speed
int motorSpeedOpen = 255; // 255/255

// Forward declarations
void MotorOxygenForward(int speed);
void MotorOxygenBackward(int speed);
void MotorOxygenStop();
void MotorFuelForward(int speed);
void MotorFuelBackward(int speed);
void MotorFuelStop();

// Pin Setup
void setup() {
  pinMode(motorOxygenEN, OUTPUT);
  pinMode(motorOxygenIN1, OUTPUT);
  pinMode(motorOxygenIN2, OUTPUT);
  pinMode(motorFuelEN, OUTPUT);
  pinMode(motorFuelIN1, OUTPUT);
  pinMode(motorFuelIN2, OUTPUT);
  pinMode(buttonStart, INPUT);
}

// Main Loop
void loop() {
  if (digitalRead(buttonStart) == HIGH) {
    // Open valves
    MotorOxygenForward(motorSpeedOpen);
    MotorFuelForward(motorSpeedOpen);
    delay(motorTime); // Added missing semicolon
    MotorOxygenStop();
    MotorFuelStop();

    // Wait for test duration
    unsigned long startTime = millis();
    while (millis() - startTime < waitTime) {
      // If button is pressed, stop test
      if (digitalRead(buttonStart) == LOW) {
        MotorOxygenBackward(motorSpeedRetrive);
        MotorFuelBackward(motorSpeedRetrive);
        // Wait for fuse to burn
        return;
      }
    }

    // Close valves
    MotorOxygenBackward(motorSpeedRetrive);
    MotorFuelBackward(motorSpeedRetrive);
    // Wait for fuse to burn
  }
}

// Motor Control Functions
void MotorOxygenForward(int speed) { // Added type for parameter
  analogWrite(motorOxygenEN, speed); // Set speed
  digitalWrite(motorOxygenIN1, HIGH);
  digitalWrite(motorOxygenIN2, LOW);
}

void MotorOxygenBackward(int speed) { // Added type for parameter
  analogWrite(motorOxygenEN, speed); // Set speed
  digitalWrite(motorOxygenIN1, LOW);
  digitalWrite(motorOxygenIN2, HIGH);
}

void MotorOxygenStop() {
  analogWrite(motorOxygenEN, 0); // Set speed to 0%
  digitalWrite(motorOxygenIN1, LOW);
  digitalWrite(motorOxygenIN2, LOW);
}

void MotorFuelForward(int speed) { // Added type for parameter
  analogWrite(motorFuelEN, speed); // Set speed
  digitalWrite(motorFuelIN1, HIGH);
  digitalWrite(motorFuelIN2, LOW);
}

void MotorFuelBackward(int speed) { // Added type for parameter
  analogWrite(motorFuelEN, speed); // Set speed
  digitalWrite(motorFuelIN1, LOW);
  digitalWrite(motorFuelIN2, HIGH);
}

void MotorFuelStop() {
  analogWrite(motorFuelEN, 0); // Set speed to 0%
  digitalWrite(motorFuelIN1, LOW);
  digitalWrite(motorFuelIN2, LOW);
}