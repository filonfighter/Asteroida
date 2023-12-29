#include <Arduino.h>

// Following code works so when the button is pressed the start begins opening Oxygen and Fuel valves
// After specified time the valves close and the test is over
// If the button is pressed during the test, the valves close and the test is over
// This is made specifically for the BTS7960B motor driver

// Motor Control Pins
int motorOxygenCW_EN = 39; // Motor Oxygen Enable Pin
int motorOxygenCCW_EN = 36; // Motor Oxygen Enable Pin
int motorOxygenSpeed = 34; // Motor Oxygen Speed input (must be PWM)

int motorFuelCW_EN = 32; // Motor Fuel Enable Pin
int motorFuelCCW_EN = 35; // Motor Fuel Enable Pin
int motorFuelSpeed = 33; // Motor Fuel Speed input (must be PWM)

// Button Pins
int buttonStart = 25; // Start Button Pin

// Igniter Pin
int igniter = 26; // Igniter Pin

// Test duration in miliseconds
unsigned long waitTime = 5000; // 5 second

// Motor Open/Close time in miliseconds
unsigned long motorOxygenOpeningTime = 1000; // 1 second
unsigned long motorOxygenClosingTime = 2000; // 2 seconds
unsigned long motorFuelOpeningTime = 1000; // 1 second
unsigned long motorFuelClosingTime = 3000; // 3 seconds

// Igniter time in miliseconds
unsigned long igniterTime = 1000; // 1 second

// Motor closing speed
int motorSpeedClose = 200; // 200/255

// Motor opening speed
int motorSpeedOpen = 255; // 255/255

// Forward declarations
void MotorOxygenOpen(int speed);
void MotorOxygenClose(int speed);
void MotorOxygenStop();
void MotorFuelOpen(int speed);
void MotorFuelClose(int speed);
void MotorFuelStop();
void Ignite(int time);

// Test permission
bool testPermission = true;

// Pin Setup
void setup() {
  pinMode(motorOxygenCW_EN, OUTPUT);
  pinMode(motorOxygenCCW_EN, OUTPUT);
  pinMode(motorOxygenSpeed, OUTPUT);
  pinMode(motorFuelCW_EN, OUTPUT);
  pinMode(motorFuelCCW_EN, OUTPUT);
  pinMode(motorFuelSpeed, OUTPUT);
  pinMode(buttonStart, INPUT);
  pinMode(igniter, OUTPUT);
}

// Main Loop
void loop() {
  if (digitalRead(buttonStart) == HIGH && testPermission == true) {
    // Open valves
    MotorOxygenOpen(motorSpeedOpen);
    delay(motorOxygenOpeningTime);
    MotorOxygenStop();

    MotorFuelOpen(motorSpeedOpen);
    delay(motorFuelOpeningTime);
    MotorFuelStop();

    // Igniter part
    Ignite(igniterTime); // Writes High to the igniter pin

    // Wait for test duration
    unsigned long startTime = millis();
    while (millis() - startTime < waitTime) {
      // If button is pressed, stop test
      if (digitalRead(buttonStart) == LOW) {
        MotorOxygenClose(motorSpeedClose);
        delay(motorOxygenClosingTime);
        MotorOxygenStop();

        MotorFuelClose(motorSpeedClose);
        delay(motorFuelClosingTime);
        MotorFuelStop();

        testPermission = false;
        return;
      }
    }

    // Close valves
    MotorOxygenClose(motorSpeedClose);
    delay(motorOxygenClosingTime);
    MotorOxygenStop();

    MotorFuelClose(motorSpeedClose);
    delay(motorFuelClosingTime);
    MotorFuelStop();

    testPermission = false;
  }
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

void MotorFuelOpen(int speed) {
  analogWrite(motorFuelSpeed, speed); // Set speed
  digitalWrite(motorFuelCW_EN, HIGH);
  digitalWrite(motorFuelCCW_EN, LOW);
}

void MotorFuelClose(int speed) {
  analogWrite(motorFuelSpeed, speed); // Set speed
  digitalWrite(motorFuelCW_EN, LOW);
  digitalWrite(motorFuelCCW_EN, HIGH);
}

void MotorFuelStop() {
  analogWrite(motorFuelSpeed, 0); // Set speed to 0%
  digitalWrite(motorFuelCW_EN, LOW);
  digitalWrite(motorFuelCCW_EN, LOW);
}

void Ignite(int time) {
  digitalWrite(igniter, HIGH);
  delay(time);
  digitalWrite(igniter, LOW);
}