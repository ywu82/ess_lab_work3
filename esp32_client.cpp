#include <WiFi.h>

// WiFi credentials
const char* ssid = "";
const char* password = "";

// Server address and port
const char* host = "10.0.0.161";   // Target server IP
const uint16_t port = 8080;        // Target port

void setup() {
  Serial.begin(115200); // Start serial communication

  // Connect to WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");  // Wait until connected
  }
  Serial.println("WiFi connected.");

  // Create a TCP client and connect to the server
  WiFiClient client;
  if (client.connect(host, port)) {
    Serial.println("Connected to server.");

    // Send a simple message
    client.println("Hello from ESP32-S3 Client!");

    // Close the connection
    client.stop();
    Serial.println("Message sent, disconnected.");
  } else {
    Serial.println("Connection failed.");
  }
}

void loop() {
  // Nothing to do in the main loop
}
