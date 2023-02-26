#include <esp_now.h>
#include <WiFi.h>

#define STRAIN_PIN 34 // Pin connected to the strain gauge resistor
#define THRESHOLD 500 // Threshold voltage to trigger command
#define CHANNEL 1 // ESP-Now channel to use

uint8_t mac[] = {0xEC, 0x62, 0x60, 0x1D, 0x24, 0x88}; // MAC address of client

void setup() {
  Serial.begin(115200);

  // Set up ESP-Now
  WiFi.mode(WIFI_STA); // set Wi-Fi mode to station (client) mode
  if (esp_now_init() != ESP_OK) { // initialize ESP-Now library
    Serial.println("Error initializing ESP-Now"); // print error message if initialization fails
    return;
  }

  // Add the client to the ESP-Now peer list
  esp_now_peer_info_t peerInfo;
  memset(&peerInfo, 0, sizeof(peerInfo));
  peerInfo.channel = CHANNEL;
  memcpy(peerInfo.peer_addr, mac, 6); // set the peer MAC address
  if (esp_now_add_peer(&peerInfo) != ESP_OK) { // add the peer to the list of peers
    Serial.println("Failed to add peer"); // print error message if adding fails
    return;
  }
  
  Serial.println("ESP-32 Sender client ready");
}

void loop() {
  // Read voltage from strain gauge resistor
  int voltage = analogRead(STRAIN_PIN); // read the analog voltage from the strain gauge

  // If voltage exceeds threshold, send command to client
  if (voltage > THRESHOLD) { // check if the voltage exceeds the threshold
    uint8_t data = 1; // Command data to send
    esp_err_t result = esp_now_send(mac, &data, sizeof(data)); // send command to client with the given MAC address
    if (result == ESP_OK) { // check if the command was sent successfully
      Serial.println("Command sent"); // print message indicating command was sent
    } else {
      Serial.println("Failed to send command"); // print error message if sending fails
    }
  }
  
  delay(1000); // Wait 1 second before checking again
}

/*
First, the necessary libraries are included: esp_now.h for ESP-Now functionality and WiFi.h for Wi-Fi functionality.

Then, the pin connected to the strain gauge resistor is defined as STRAIN_PIN, and the threshold voltage to trigger a command is set as THRESHOLD. The ESP-Now channel to use is set as CHANNEL.

The MAC address of the client is defined as mac. This is necessary for the server to send commands to the correct client.

In the setup() function, the ESP-Now mode is set to WIFI_STA, which is station mode for Wi-Fi connectivity. Then, ESP-Now is initialized. If there is an error initializing ESP-Now, the function returns.

Next, the client is added to the ESP-Now peer list. The esp_now_peer_info_t struct is used to store information about the peer, including the channel to use and the MAC address of the peer. If there is an error adding the peer to the list, the function returns.

In the loop() function, the voltage from the strain gauge resistor is read using the analogRead() function. If the voltage exceeds the threshold, a command is sent to the client using ESP-Now.

The command data to send is defined as data. This is a uint8_t variable with a value of 1. If the command is successfully sent using esp_now_send(), a message is printed to the serial monitor indicating that the command was sent. 

If there is an error sending the command, a message is printed to the serial monitor indicating that the command failed to send.

Finally, there is a delay() of 1 second before checking the voltage again. This prevents the loop from running too quickly and potentially overwhelming the ESP-Now network.
*/