/*Iowa State Formula SAE Electrical Subsystem*/

#include <esp_now.h>
#include <WiFi.h>
#include <stdio.h>
#include <string>
#include <iostream>

// MAC_Addresses for the different ESP32 Boards                         // Board Identifiers
// uint8_t broadcastAddress1[] = {0x24, 0x0A, 0xC4, 0xEE, 0x7D, 0x04};  // Long Antenna
// uint8_t broadcastAddress1[] = {0x30, 0xC6, 0xF7, 0x20, 0x50, 0x2C};  // ESP CAM 1 (antenna board)
// uint8_t broadcastAddress1[] = {0xEC, 0x62, 0x60, 0x9D, 0xB4, 0x50};  // White Tape Devkit AB
uint8_t broadcastAddress1[] = {0xEC, 0x62, 0x60, 0x9D, 0x94, 0x5C};  // No White Tape Devkit AB
// uint8_t broadcastAddress1[] = {0x08, 0x3A, 0xF2, 0xB7, 0x70, 0xD0};  // Blue Tape Devkit
// uint8_t broadcastAddress1[] = {0x40, 0x91, 0x51, 0xAC, 0x2E, 0x54};  // No Tape Devkit

// Data Structure to store variables to be sent
// struct must match receiver
typedef struct data_struct {
  float RPM;        // RPM value
  float TPS;        // TPS value
  float FOT;        // Fuel Open Time value
  float IA;         // Ignition Angle value
  float Lam;        // Lambda value
  float AirT;       // Air Temp value
  float CoolT;      // Coolent Temp value
  float Lat;        // Latitude
  float Lng;        // Longitude
  float Speed;      // GPS Speed
  float OilP;       // Oil Pressure
  float FuelP;      // Fuel Pressure
  float FLTemp;     // Front Left Brake Temp
  float FRTemp;     // Front Right Brake Temp
  float RLTemp;     // Rear Left Brake Temp
  float RRTemp;     // Rear Right Brake Temp
  float FRPot;      // Front Right suspension damper
  float FLPot;      // Front Left suspension damper
  float RRPot;      // Rear Right suspension damper
  float RLPot;      // Rear Left suspension damper
  float BrakeFront; // Front Brake Pressure
  float BrakeRear;  // Rear Brake Pressure
  float BrakeBias;  // Brake Bias
  float AccX;       // Acclerometer X Axis
  float AccY;       // Acclerometer Y Axis
  float AccZ;       // Acclerometer Z Axis
  float GyrX;       // Gyroscope X Axis
  float GyrY;       // Gyroscope Y Axis
  float GyrZ;       // Gyroscope Z Axis
  float MagX;       // Magnetometer X Axis
  float MagY;       // Magnetometer Y Axis
  float MagZ;       // Magnetometer Z Axis
} data_struct;
data_struct telemetry;

// Function to check if data send was successful
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "" : "Delivery Fail");
}

void setup() {
  Serial.begin(115200); // Serial Monitor printout
  Serial2.begin(115200); // Serial port for the wireless connection
  WiFi.mode(WIFI_STA);
  ET.begin(details(telemetry), &Serial2);

  // Checks for ESP connection
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  esp_now_register_send_cb(OnDataSent);

  // Set Receiver board information
  esp_now_peer_info_t peerInfo; // Create object type for receiver board
  peerInfo.channel = 0;
  peerInfo.encrypt = false;
  memcpy(peerInfo.peer_addr, broadcastAddress1, 6);
  // Check if Reciever was connected successfully
  if (esp_now_add_peer(&peerInfo) != ESP_OK) {
    Serial.println("Failed to add peer");
    return;
  }
}

void loop() {
  // Get byte data from arduino main board
  byte *receivedByteData = (byte *)&telemetry;
  Serial2.readBytes(receivedByteData, sizeof(telemetry));

  // sends the data
  esp_err_t result = esp_now_send(0, (uint8_t *)&telemetry, sizeof(telemetry));

  // Tests that the data was sent successfully and either prints an Error or the Data that was sent
  if (result == ESP_OK) {
    // Print out the data for debug
    Serial.printf("%f, %f, %f, %f, %f, %f, %f, %f\n", telemetry.TPS, telemetry.FRTemp, telemetry.AccX, telemetry.AccY, telemetry.AccZ, telemetry.GyrX, telemetry.GyrY, telemetry.GyrZ);
  }
  else {
    Serial.println("ERROR: problem sending the data");
  }
  // delay for stability
  delay(1);
}
