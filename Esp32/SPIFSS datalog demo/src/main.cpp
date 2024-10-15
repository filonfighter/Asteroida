#include <Arduino.h>
#include "SPIFFS.h"

// demo input pin
int loud_pin = 34;
 
void setup() {
  // demo setup
  Serial.begin(115200);
  pinMode(loud_pin, INPUT);
  
  // Mount SPIFFS Only to perform once at the beginning. Afer mounting on a device you can read and write files.
  if (SPIFFS.begin(true)) {
    Serial.println("SPIFFS mounted successfully.");
} else {
    Serial.println("SPIFFS mount failed. Check your filesystem.");
}

}

 
void loop() {
  
  // demo read sensor data
  int sensorData = analogRead(loud_pin);

  // writing to SPIFFS files
  File logFile = SPIFFS.open("/datalog.txt", "a");

  // example of writing sensor data structure to a file
  if (logFile) {
    logFile.print(millis());
    logFile.print(", ");
    logFile.print(sensorData);
    logFile.print(", ");
    logFile.println();
    logFile.close();
} else {
    Serial.println("Failed to open log file for writing.");
  }

}
