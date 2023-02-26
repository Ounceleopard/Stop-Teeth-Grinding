/*
Brayan Villatoro FEB 2023 
Version 2.9

This code functions as the reciever to the server ESP32 located inside the mouth of the indivdual
this client takes in the values sent and triggers a TENS device

*/
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>

BLECharacteristic* pCharacteristic;
bool isConnected = false;

const int tensPin = 4; // Tens trigger, In testing try 0 through 5 

class MyServerCallbacks : public BLEServerCallbacks {
  void onConnect(BLEServer* pServer) {
    isConnected = true;
    Serial.println("Connected to central"); //ESP Sender 
  }

  void onDisconnect(BLEServer* pServer) {
    isConnected = false;
    Serial.println("Disconnected from central"); //ESP Sender 
  }
};

void setup() {
  Serial.begin(115200);

  BLEDevice::init("ESP32 Receiver"); // Set device name AKA the Client to the Server 

  // Create BLE Server
  BLEServer* pServer = BLEDevice::createServer();
  pServer->setCallbacks(new MyServerCallbacks());

  BLEService* pService = pServer->createService(BLEUUID("4fafc201-1fb5-459e-8fcc-c5c9c331914b")); // MATCH TO ESP32 SENDER 

  // Create BLE Characteristic
  pCharacteristic = pService->createCharacteristic(BLEUUID("beb5483e-36e1-4688-b7f5-ea07361b26a8"), BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_WRITE | BLECharacteristic::PROPERTY_NOTIFY);

  // Start the service
  pService->start();
  
  // Base code to build on a possible phone remote trigger 
  // Start advertising
  BLEAdvertising* pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(pService->getUUID());
  pAdvertising->setScanResponse(true);
  pAdvertising->setMinPreferred(0x06);  // functions that help with iPhone connections issue
  pAdvertising->setMinPreferred(0x12);
  pAdvertising->start();
}

void loop() {
  // Wait for central connection
  while (!isConnected) {
    delay(10);
  }

  // Wait for TENS trigger
  while (pCharacteristic->getValue() != "TENS ON") {
    delay(10);
  }

  Serial.println("TENS Triggered");

  // Using digitalWrite A5 is set HIGH for 1 sec after which it will become LOW for 1 sec 
  // Activate TENS device
  digitalWrite(tensPin, HIGH); // 3.3V/5V depending on placement 
  delay(1000); // 1 Second period, test sync during testings with ESP 32 sender 
  digitalWrite(tensPin, LOW); // A0 to A5 

  // Confirm TENS trigger
  pCharacteristic->setValue("TENS CONFIRMED");
  pCharacteristic->notify(); // ESP32 sender confirm

  // Wait for TENS confirmation
  while (pCharacteristic->getValue() != "TENS OFF") { // ESP32 sender strain guage has fallen into value.
    delay(10); // 0.01 seconds or 10 miliseconds check 
  Serial.println("TENS Confirmed");

  // Clear TENS confirmation
  pCharacteristic->setValue("");
  pCharacteristic->notify();
  }
}