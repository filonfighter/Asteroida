#include <Arduino.h>

// Following code works so when the button is pressed the start begins opening Oxygen and Fuel valves
// After specified time the valves close and the test is over
// If the button is pressed during the test, the valves close and the test is over

//Test restart variable

int x=0;


// Motor Control Pins
int motorOxygenCW_EN = 34; // Motor Oxygen Enable Pin
int motorOxygenCCW_EN = 33; // Motor Oxygen Enable Pin
int motorOxygenSpeed = 32; // Motor Oxygen Speed input (must be PWM)

int motorFuelCW_EN = 35; // Motor Fuel Enable Pin
int motorFuelCCW_EN = 25; // Motor Fuel Enable Pin
int motorFuelSpeed = 36; // Motor Fuel Speed input (must be PWM)

// Button Pins
int buttonStart = 27; // Start Button Pin

// Test duration in miliseconds
unsigned long waitTime = 5000; // 5 second

// Fuel Motor Open time in miliseconds
unsigned long FuelmotorTimeOpen = 1000; // 1 second

// Oxygen Motor Open time in miliseconds
unsigned long OxygenmotorTimeOpen = 1000; // 1 second

// Fuel Motor Close time in miliseconds
unsigned long FuelmotorTimeClose = 1500; // 1.5 seconds but that is still a thing that is going to be measured

// Oxygen Motor Close time in miliseconds
unsigned long OxygenmotorTimeClose = 1500; // 1.5 seconds but that is still a thing that is going to be measured

// Motor closing speed
int motorSpeedClose = 200; // 200/255

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
  pinMode(motorOxygenCW_EN, OUTPUT);
  pinMode(motorOxygenCCW_EN, OUTPUT);
  pinMode(motorOxygenSpeed, OUTPUT);
  pinMode(motorFuelCW_EN, OUTPUT);
  pinMode(motorFuelCCW_EN, OUTPUT);
  pinMode(motorFuelSpeed, OUTPUT);
  pinMode(buttonStart, INPUT);
}

// Main Loop
void loop() {
  if ((digitalRead(buttonStart) == HIGH) && (x==0)) {
    // Open valves
    MotorFuelForward(motorSpeedOpen);
    delay(FuelmotorTimeOpen); // Added missing semicolon
    MotorFuelStop();
	MotorOxygenForward(motorSpeedOpen);
	delay(OxygenmotorTimeOpen); // Added missing semicolon
    MotorOxygenStop();
    
	
	// Tutaj jeszcze trzeba dodać część z włączeniem ignitera ale nie wiadomo jeszcze do jakich pinow do podepniemy to nic jeszcze nie pisze

    // Wait for test duration
    unsigned long startTime = millis();
    while (millis() - startTime < waitTime) {
      // If button is pressed, stop test
      if (digitalRead(buttonStart) == LOW) {
			MotorOxygenBackward(motorSpeedClose);
			delay(OxygenmotorTimeClose);
			MotorOxygenStop();
			MotorFuelBackward(motorSpeedClose);  // Wait for fuse to burn
			delay(FuelmotorTimeClose);									
			MotorFuelStop();
        // Wait for fuse to burn
        return;
      }
    }

    // Close valves
    MotorOxygenBackward(motorSpeedClose);
	delay(OxygenmotorTimeClose);
	MotorOxygenStop();
	MotorFuelBackward(motorSpeedClose);  // Wait for fuse to burn
	delay(FuelmotorTimeClose);									
	MotorFuelStop();
	x=1;
	
	
	
	
  }
  
  if (digitalRead(buttonRESTART) == HIGH) {

		x=0;

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