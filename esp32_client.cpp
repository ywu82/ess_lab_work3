#include <WiFi.h>

const char* ssid = "WIFI-4AA4";
const char* password = "comedy4917equal";

const char* host = "10.0.0.161";
const uint16_t port = 8080;

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("WiFi connected.");

  WiFiClient client;
  if (client.connect(host, port)) {
    Serial.println("Connected to server.");
    client.println("Hello from ESP32-S3 Client!");
    client.stop();
    Serial.println("Message sent, disconnected.");
  } else {
    Serial.println("Connection failed.");
  }
}

void loop() {}
