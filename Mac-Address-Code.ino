#include "WiFi.h"

void setup() {
  Serial.begin(115200);
  delay(3000); // Delayed in order to not miss it when grabbing it from serial monitor 

  // Initialize Wi-Fi
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();

  // Print the MAC address of the ESP32
  Serial.println(WiFi.macAddress());
}

void loop() {}
