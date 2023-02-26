/*
Brayan Villatoro FEB 2023 
Version 3.6

This code functions as the server to the server ESP32 located on the indivdual's night stand 
this server takes in the values from a guage reisistor and sents it out to the client in order to trigger a TENS
*/
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>

BLECharacteristic* pCharacteristic;
bool isConfirmed = false;

const int strainGaugePin = 34; // Strain Gauge analog input pin
const int triggerVoltage = 1500; // Strain Gauge threshold to trigger Tens = 1500  millivolts AKA 1.5 volts
const int triggerTime = 1000; // Tens trigger time in milliseconds = 1 Second 

class MyCallbacks : public BLECharacteristicCallbacks {
  void onWrite(BLECharacteristic* pChar) {
    isConfirmed = true;
  }
};

void setup() {
  Serial.begin(115200); // Serial data transmission
  Serial.println("Starting BLE work!"); // BLE is turning on 

  BLEDevice::init("ESP32 Sender"); // Set device name AKA the Server 

  // Create BLE Server
  BLEServer* pServer = BLEDevice::createServer();
  BLEService* pService = pServer->createService(BLEUUID("4fafc201-1fb5-459e-8fcc-c5c9c331914b")); // MATCH TO ESP 32 Client 

  // Create BLE Characteristic, Handshake
  pCharacteristic = pService->createCharacteristic(BLEUUID("beb5483e-36e1-4688-b7f5-ea07361b26a8"), BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_WRITE | BLECharacteristic::PROPERTY_NOTIFY);

  // Set characteristic callback
  pCharacteristic->setCallbacks(new MyCallbacks());

  // Start the service
  pService->start();

  // Start advertising
  BLEAdvertising* pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(pService->getUUID());
  pAdvertising->start();

  Serial.println("Advertising started!"); // BLE is visible 
}

void loop() {
  int strainGaugeValue = analogRead(strainGaugePin); // Function to trigger actions 
  Serial.println(strainGaugeValue); // Live feeed of strain gauge values this can help adjust the gauge in testing

  if (strainGaugeValue > triggerVoltage) { // Requires testing on how the values will be recieved 
    // Trigger tens device
    Serial.println("Triggering Tens Device");
    pCharacteristic->setValue("TENS ON");
    pCharacteristic->notify();
    delay(triggerTime);
    pCharacteristic->setValue("TENS OFF");
    pCharacteristic->notify();

    // Wait for confirmation
    while (!isConfirmed) { // Handshake with ESP 32 client 
      delay(10);
    }
    Serial.println("Confirmation received");
    isConfirmed = false; // Reset confirmation flag
  }

  delay(1000); // Wait for 1 second before reading again
}
