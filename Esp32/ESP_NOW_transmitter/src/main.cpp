// importing ardiuno framework
#include <Arduino.h>
// importing WiFi library for ESP-NOW
#include <WiFi.h>
#include <esp_now.h>  // ESP-NOW library
// importing SPIFFS library for file system
#include <SPIFFS.h>
// imporing HX711 library for load cell
#include <HX711_ADC.h>


// reciving board mac address
uint8_t broadcastAddress[] = {0xC8, 0xF0, 0x9E, 0xF2, 0x52, 0xF4}; // Board 3 (reciver) MAC Address

// structure for sending data
typedef struct struct_message {
  float raw_thrust;
  bool board_status;
} struct_message;

// creating object of struct_message
struct_message myData;

// structure for peer information
esp_now_peer_info_t peerInfo;

// callback when data is sent
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("\r\nLast Packet Send Status:\t");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}

// Pins for HX711
const int HX711_dout = 4; // Pin DOUT HX711 to MCU
const int HX711_sck = 5;  // Pin SCK HX711 to MCU

// Constructor HX711
HX711_ADC LoadCell(HX711_dout, HX711_sck);

// thrust variable
float thrust;

void setup(){
  // Init HX711
  LoadCell.begin();
  float calibrationValue = 696.0; // Calibration value for load cell
  LoadCell.setCalFactor(calibrationValue); // Calibrate the load cell
  unsigned long stabilizingtime = 2000; // Stabilization time in milliseconds
  boolean _tare = true; // Tare
  LoadCell.start(stabilizingtime, _tare);
  if (LoadCell.getTareTimeoutFlag()) {
    Serial.println("Timeout, sprawdź połączenie MCU>HX711");
    while (1);
  } else {
    Serial.println("Startup is complete");
  }

  // Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);

  // Init ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  // Once ESPNow is successfully Init, we will register for Send CB to
  // get the status of Trasnmitted packet
  esp_now_register_send_cb(OnDataSent);
  
  // Register peer
  memcpy(peerInfo.peer_addr, broadcastAddress, 6);
  peerInfo.channel = 0;  
  peerInfo.encrypt = false;
  
  // Add peer        
  if (esp_now_add_peer(&peerInfo) != ESP_OK){
    Serial.println("Failed to add peer");
    return;
  }
  
  // Serial communication for debugging
  Serial.begin(115200);
  
  // Mount SPIFFS, check if it was uploaded before!
  if (SPIFFS.begin(true)) {
    Serial.println("SPIFFS mounted successfully.");
} else {
    Serial.println("SPIFFS mount failed. Check your filesystem.");
}
}
 
void loop(){
  
  // Load cell handling
  if (LoadCell.update()) {
    float thrust = LoadCell.getData(); // data from load cell
    myData.raw_thrust = thrust; // Data to send
    Serial.print("Load_cell output val: ");
    Serial.println(thrust);
  }

  // log sensor data
  File logFile = SPIFFS.open("/datalog.txt", "a");

  // write to file in CSV format
  if (logFile) {
    logFile.close();
} else {
    Serial.println("Failed to open log file for writing.");
  }

  // Set values to send
  myData.raw_thrust, thrust;
  myData.board_status = true;
  
  // Send message via ESP-NOW
  esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *) &myData, sizeof(myData));
   
  if (result == ESP_OK) {
    Serial.println("Sent with success");
  }
  else {
    Serial.println("Error sending the data");
  }
}