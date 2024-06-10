#include <Arduino.h>
#include "SPIFFS.h"

void setup() {
  Serial.begin(115200);
  
  // initialization of SPIFFS
  if (!SPIFFS.begin(true)) {
    Serial.println("An Error has occurred while mounting SPIFFS");
    return;
  }

  // waiting for a command from the serial monitor
  char command = 0;
  while (command != 'S') {
    if (Serial.available() > 0) {
      command = Serial.read();
    }
  }
  // reading the data from the file
  File fileToRead = SPIFFS.open("/datalog.txt", "r");
  if (fileToRead) {
    while (fileToRead.available()) {
      Serial.write(fileToRead.read());
    }
    fileToRead.close();
  } else {
    Serial.println("Failed to open file for reading.");
  }
}

void loop() {
  // put your main code here, to run repeatedly:
}