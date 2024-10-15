#include <Arduino.h>


// Interrupt pins
const int ABORT = 34;
const int START = 35;

// time variables for motor control
volatile bool isMotorOpening = false;
volatile bool isMotorClosing = false;

// First motor control variables (Oxygen)
const int PUL_PIN = 14;  // Pulse pin
const int DIR_PIN = 27;  // Direction pin

// Second motor control variables (Fuel)
const int PUL_PIN2 = 12;  // Pulse pin
const int DIR_PIN2 = 13;  // Direction pin

// Oxygen motor control channel
const int PWM_CHANNEL = 0;

// Fuel motor control channel
const int PWM_CHANNEL2 = 1;

const int FREQUENCY = 200;
const int RESOLUTION = 8;

void motorControl(int motor_dirpin, int motor_channel, bool open) {
  digitalWrite(motor_dirpin, open ? HIGH : LOW);
  ledcWrite(motor_channel, 127);
  unsigned long startTime = millis();
  while (millis() - startTime < 1000) {
    // Non-blocking delay
  }
  ledcWrite(motor_channel, 0);
}

void hotfire() {
  motorControl(DIR_PIN, PWM_CHANNEL, true);  // Open oxygen valve
  motorControl(DIR_PIN2, PWM_CHANNEL2, true);  // Open fuel valve
  unsigned long startTime = millis();
  while (millis() - startTime < 9000) {
    // Non-blocking delay
  }
  motorControl(DIR_PIN, PWM_CHANNEL, false);  // Close oxygen valve
  motorControl(DIR_PIN2, PWM_CHANNEL2, false);  // Close fuel valve
}

void abortTest() {
  motorControl(DIR_PIN, PWM_CHANNEL, false);
  motorControl(DIR_PIN2, PWM_CHANNEL2, false);
}

void ISRStart() {
  static unsigned long lastInterruptTime = 0;
  if (millis() - lastInterruptTime > 200) {  // Debounce time of 200ms
    hotfire();
  }
  lastInterruptTime = millis();
}

void ISRAbort() {
  static unsigned long lastInterruptTime = 0;
  if (millis() - lastInterruptTime > 200) {  // Debounce time of 200ms
    abortTest();
  }
  lastInterruptTime = millis();
}

void setup() {
  pinMode(PUL_PIN, OUTPUT);
  pinMode(DIR_PIN, OUTPUT);
  pinMode(PUL_PIN2, OUTPUT);
  pinMode(DIR_PIN2, OUTPUT);

  ledcSetup(PWM_CHANNEL, FREQUENCY, RESOLUTION);
  ledcSetup(PWM_CHANNEL2, FREQUENCY, RESOLUTION);
  ledcAttachPin(PUL_PIN, PWM_CHANNEL);
  ledcAttachPin(PUL_PIN2, PWM_CHANNEL2);

  pinMode(ABORT, INPUT);
  pinMode(START, INPUT);

  attachInterrupt(digitalPinToInterrupt(START), ISRStart, RISING);
  attachInterrupt(digitalPinToInterrupt(ABORT), ISRAbort, RISING);
}

void loop() {
  // Main loop does nothing, waiting for interrupts
}