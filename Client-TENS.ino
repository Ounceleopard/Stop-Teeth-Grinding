#include <esp_now.h> // ESP-Now library
#include <WiFi.h> // WiFi library

#define TENS_PIN 26 // Pin connected to tens charge
#define CHANNEL 1 // ESP-Now channel to use

uint8_t mac[] = {0xC8, 0xF0, 0x9E, 0x4E, 0x55, 0xB0}; // MAC address of server

void setup() {
  Serial.begin(115200); // Start serial communication for debugging

  // Set up ESP-Now
  WiFi.mode(WIFI_STA); // Set WiFi mode to station mode
  if (esp_now_init() != ESP_OK) { // Initialize ESP-Now
    Serial.println("Error initializing ESP-Now");
    return;
  }

  // Register callback function for receiving commands
  esp_now_register_recv_cb([](const uint8_t *mac, const uint8_t *data, int len) {
    if (len == 1 && data[0] == 1) { // Check if command is received with value 1
      // Command received, trigger tens charge
      digitalWrite(TENS_PIN, HIGH);
      delay(100); // Tens charge burst duration, 1000 = 1 Second from high to low burst 
      digitalWrite(TENS_PIN, LOW);

      Serial.println("Tens charge triggered");
    }
  });

  // Add the server to the ESP-Now peer list
  esp_now_peer_info_t peerInfo;
  memset(&peerInfo, 0, sizeof(peerInfo));
  peerInfo.channel = CHANNEL;
  memcpy(peerInfo.peer_addr, mac, 6);
  if (esp_now_add_peer(&peerInfo) != ESP_OK) { // Add the server as a peer
    Serial.println("Failed to add peer");
    return;
  }

  pinMode(TENS_PIN, OUTPUT); // Set TENS_PIN as an output
  Serial.println("ESP-32 client ready");
}

void loop() {
  // Do nothing, waiting for commands to be received
}

/*
The code starts by including the necessary libraries for ESP-Now and WiFi. It then defines the TENS_PIN and CHANNEL constants, which represent the GPIO pin connected to the TENS charge and the ESP-Now channel to use, respectively.

Next, the code defines the MAC address of the server that it will connect to. This address should match the MAC address of the ESP32 running the server code.

In the setup function, the code sets up ESP-Now by initializing it and registering a callback function to handle incoming commands. The callback function checks if the received command has a length of 1 and a value of 1, 

indicating that the TENS charge should be triggered.

The code then adds the server as a peer to the ESP-Now peer list, and sets the TENS_PIN as an output. Finally, the code prints a message to the serial monitor indicating that the client is ready.

In the loop function, the code does nothing and simply waits for incoming commands.
*/

