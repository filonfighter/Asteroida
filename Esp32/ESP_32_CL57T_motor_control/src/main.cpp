#include <Arduino.h>

const int PUL_PIN = 14;  // Pulse pin
const int DIR_PIN = 27;  // Direction pin
const int ENA_PIN = 26;  // Enable pin
const int PWM_CHANNEL = 0;
const int FREQUENCY = 200;  // 200 Hz for 200 steps per second
const int RESOLUTION = 8;   // 8-bit resolution

void setup() {
  // Set up the pins
  pinMode(DIR_PIN, OUTPUT);
  pinMode(ENA_PIN, OUTPUT);
  
  // Enable the driver
  digitalWrite(ENA_PIN, HIGH);
  
  // Set initial direction
  digitalWrite(DIR_PIN, HIGH);

  // Configure PWM
  ledcSetup(PWM_CHANNEL, FREQUENCY, RESOLUTION);
  ledcAttachPin(PUL_PIN, PWM_CHANNEL);
}

void loop() {
  // Set initial duty cycle to 50%
  ledcWrite(PWM_CHANNEL, 128);  // 50% duty cycle

  // The motor will rotate in one direction for 3 seconds
  delay(3000);  // Keep rotating for 3 seconds
  
  // Stop the PWM signal
  ledcWrite(PWM_CHANNEL, 0);  // Stop sending PWM signal
  
  // Then disable the driver (assuming LOW actually disables it)
  digitalWrite(ENA_PIN, HIGH);

  // The motor will rotate in one direction for 3 seconds
  delay(3000);  // Keep rotating for 3 seconds

  // Then disable the driver (assuming LOW actually disables it)
  digitalWrite(ENA_PIN, LOW);

  // Set the direction to the opposite
  digitalWrite(DIR_PIN, LOW);

  // Set initial duty cycle to 50%
  ledcWrite(PWM_CHANNEL, 128);  // 50% duty cycle

  // The motor will rotate in the opposite direction for 3 seconds
  delay(3000);  // Keep rotating for 3 seconds

  // Stop the PWM signal
  ledcWrite(PWM_CHANNEL, 0);  // Stop sending PWM signal

  //change direction
  digitalWrite(DIR_PIN, HIGH);
}