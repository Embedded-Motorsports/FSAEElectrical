/*Iowa State Formula SAE Electrical Subsystem*/

#include <esp_now.h>
#include <WiFi.h>
#include<stdio.h>
#include<stdlib.h>
#include <iostream>

// Structure example to receive data
// Must match the sender structure
typedef struct data_struct {
  float RPM;        // RPM
  float TPS;        // TPS
  float FOT;        // Fuel Open Time
  float IA;         // Ignition Angle
  float Lam;        // Lambda
  float AirT;       // Air Temp
  float CoolT;      // Coolant Temp
  float Lat;        // Latitude
  float Lng;        // Longitude
  float Speed;      // GPS Speed
  float OilP;       // Oil Pressure
  float FuelP;      // Fuel Pressure
  float FLTemp;     // Front Left Brake Temp
  float FRTemp;     // Front Right Brake Temp
  float RLTemp;     // Rear Left Brake Temp
  float RRTemp;     // Rear Right Brake Temp
  float FRPot;      // Front Right Suspension Damper
  float FLPot;      // Front Left Suspension Damper
  float RRPot;      // Rear Right Suspension Damper
  float RLPot;      // Rear Left Suspension Damper
  float BrakeFront; // Front Brake Pressure
  float BrakeRear;  // Rear Brake Pressure
  float BrakeBias;  // Brake Bias
  float AccX;       // Accelerometer X Axis
  float AccY;       // Accelerometer Y Axis
  float AccZ;       // Accelerometer Z Axis
  float GyrX;       // Gyroscope X Axis
  float GyrY;       // Gyroscope Y Axis
  float GyrZ;       // Gyroscope Z Axis
  float MagX;       // Magnetometer X Axis
  float MagY;       // Magnetometer Y Axis
  float MagZ;       // Magnetometer Z Axis
} data_struct;
data_struct telemetry;

double brakeBias = 0;

// When the board recieves data:
void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) {
  // Tracks the size of the data structure
  memcpy(&telemetry, incomingData, sizeof(telemetry)); // Memory Allocation for data
}
 
void setup() {
  // Serial monitor printout for debug
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);

  // init the ESP connection
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  // Register Reciever with the Sender
  esp_now_register_recv_cb(OnDataRecv);
}
 
void loop() {
  // CSV format Serial Print
  
  brakeBias = (.99 * telemetry.BrakeFront) / ((.99 * telemetry.BrakeFront) + (0.79 * telemetry.BrakeRear));
  Serial.printf("%f, %f, %f, %f, %f, %f, %f, ", telemetry.RPM, telemetry.TPS, telemetry.FOT, telemetry.IA, telemetry.Lam, telemetry.AirT, telemetry.CoolT);
  Serial.printf("%f, %f, %f, %f, %f, %f, %f, %f, ", telemetry.Lat, telemetry.Lng, telemetry.Speed, telemetry.OilP, telemetry.FuelP, telemetry.FLTemp, telemetry.FRTemp, telemetry.RLTemp);
  Serial.printf("%f, %f, %f, %f, %f, %f, %f, %f, ", telemetry.RRTemp, telemetry.FRPot, telemetry.FLPot, telemetry.RRPot, telemetry.RLPot, telemetry.BrakeFront, telemetry.BrakeRear, telemetry.BrakeBias);
  Serial.printf("%f, %f, %f, %f, %f, %f, %f, %f, %f\n", telemetry.AccX, telemetry.AccY, telemetry.AccZ, telemetry.GyrX, telemetry.GyrY, telemetry.GyrZ, telemetry.MagX, telemetry.MagY, telemetry.MagZ);

  // delay for stability
  delay(1);
}