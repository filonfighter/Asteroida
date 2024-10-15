#include <Arduino.h>
#include "SPIFFS.h"

// demo pin
int loud_pin = 34;
 
void setup() {
  // setup
  Serial.begin(115200);
  pinMode(loud_pin, INPUT);
  
  // Mount SPIFFS
  if (SPIFFS.begin(true)) {
    Serial.println("SPIFFS mounted successfully.");
} else {
    Serial.println("SPIFFS mount failed. Check your filesystem.");
}

}

 
void loop() {
  
  // read sensor data
  int sensorData = analogRead(loud_pin);

  // log sensor data
  File logFile = SPIFFS.open("/datalog.txt", "a");

  // write to file in CSV format
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
