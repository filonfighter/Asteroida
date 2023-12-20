#include <Arduino.h>

// Poniższy program powstał do testowania ogólnej sprawności komuinikacji między komputerem a płytką ESP32

void setup() {
  Serial.begin(9600);
}

void loop() {
  Serial.println("Hello World");
  delay(1000);
}
