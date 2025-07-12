# INCS 810 ESS Lab Work 3 - ESP32 Network Communication and Webcam

This repository contains the source code used in Lab Work 3 of INCS 810 Embedded Systems Security. The tasks explore ESP32’s capability as a network client, server, and webcam streaming device.

## 🔧 Files Overview

### 🔹 Part 1.1 – ESP32 as Client

- **esp32_client.cpp**  
  This file configures the ESP32 to act as a TCP client. It connects to a designated server (running on a laptop) and communicates by sending/receiving messages.

- **pc_server.py**  
  Python script that runs on a laptop to act as the TCP server. It listens for incoming connections from the ESP32 and handles communication accordingly.

### 🔹 Part 1.2 – ESP32 as Server

- **esp32_web_server.cpp**  
  This file sets up the ESP32 as a lightweight web server. When connected, the client (browser) will receive a basic login interface served by the ESP32.

### 🔹 Part 2 – ESP32 Webcam Streaming

- **esp32_webcam.cpp**  
  Demonstrates how the ESP32 can be used with its onboard camera module to stream real-time video over a local network through a web interface.

## 📁 Folder Structure

```bash
├── esp32_client.cpp # ESP32 as client
├── esp32_web_server.cpp # ESP32 as web server
├── esp32_webcam.cpp # ESP32 webcam streaming
└── pc_server.py # PC acting as server
```


## 📌 Notes

- All ESP32 programs were compiled and flashed using Arduino IDE with proper board support packages installed.
- Make sure to update IP addresses and Wi-Fi credentials in each file before uploading to the ESP32 device.
- Webcam feature requires an ESP32 board with an onboard camera module (e.g., ESP32-CAM or ESP32-S3 with OV2640).

---

Feel free to open an issue or contact me if you encounter any problems running the code.


