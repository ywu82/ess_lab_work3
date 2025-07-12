#include "esp_camera.h"
#include <WiFi.h>
#include "esp_http_server.h"

// ✅ WiFi configuration
const char* ssid = "";
const char* password = "";

// ✅ GPIO configuration 
#define PWDN_GPIO_NUM     -1
#define RESET_GPIO_NUM    -1
#define XCLK_GPIO_NUM     15
#define SIOD_GPIO_NUM     4
#define SIOC_GPIO_NUM     5
#define Y9_GPIO_NUM       16
#define Y8_GPIO_NUM       17
#define Y7_GPIO_NUM       18
#define Y6_GPIO_NUM       12
#define Y5_GPIO_NUM       10
#define Y4_GPIO_NUM       8
#define Y3_GPIO_NUM       9
#define Y2_GPIO_NUM       11
#define VSYNC_GPIO_NUM    6
#define HREF_GPIO_NUM     7
#define PCLK_GPIO_NUM     13

httpd_handle_t stream_httpd = NULL;

// 📸 Initialize the camera configuration 
void initCamera() {
  camera_config_t config;
  config.ledc_channel = LEDC_CHANNEL_0;
  config.ledc_timer = LEDC_TIMER_0;
  config.pin_d0 = Y2_GPIO_NUM;
  config.pin_d1 = Y3_GPIO_NUM;
  config.pin_d2 = Y4_GPIO_NUM;
  config.pin_d3 = Y5_GPIO_NUM;
  config.pin_d4 = Y6_GPIO_NUM;
  config.pin_d5 = Y7_GPIO_NUM;
  config.pin_d6 = Y8_GPIO_NUM;
  config.pin_d7 = Y9_GPIO_NUM;
  config.pin_xclk = XCLK_GPIO_NUM;
  config.pin_pclk = PCLK_GPIO_NUM;
  config.pin_vsync = VSYNC_GPIO_NUM;
  config.pin_href = HREF_GPIO_NUM;
  config.pin_sccb_sda = SIOD_GPIO_NUM;
  config.pin_sccb_scl = SIOC_GPIO_NUM;
  config.pin_pwdn = PWDN_GPIO_NUM;
  config.pin_reset = RESET_GPIO_NUM;

  config.xclk_freq_hz = 20000000;  // Lower clock frequency to avoid unstable DMA 
  config.pixel_format = PIXFORMAT_GRAYSCALE;  // Grayscale image 

  // config.frame_size = FRAMESIZE_96X96;  // Small resolution to save resources 
  config.frame_size = FRAMESIZE_QVGA;       // Use QVGA (320x240) resolution
  config.jpeg_quality = 12;                 // JPEG quality (0-63), lower is better 
  config.fb_count = 1;                      // Use single framebuffer 

  Serial.println("🔧 Starting camera init...");
  esp_err_t err = esp_camera_init(&config);
  if (err != ESP_OK) {
    Serial.printf("❌ Camera init failed: 0x%x\n", err);
    return;
  }
  Serial.println("✅ Camera init success!");
}

// 📡 MJPEG video stream 
static esp_err_t stream_handler(httpd_req_t *req) {
  camera_fb_t *fb = NULL;
  esp_err_t res = ESP_OK;
  char part_buf[64];

  // Set response type as multipart MJPEG
  res = httpd_resp_set_type(req, "multipart/x-mixed-replace; boundary=frame");
  if (res != ESP_OK) return res;

  while (true) {
    fb = esp_camera_fb_get();  // Capture a frame
    if (!fb) {
      Serial.println("❌ Camera capture failed");
      return ESP_FAIL;
    }

    // ✨ Manually convert GRAYSCALE to JPEG
    uint8_t *jpg_buf = NULL;
    size_t jpg_len = 0;
    if (!frame2jpg(fb, 80, &jpg_buf, &jpg_len)) {
      Serial.println("❌ frame2jpg failed");
      esp_camera_fb_return(fb);
      continue;
    }

    // Send multipart JPEG headers and data
    snprintf(part_buf, sizeof(part_buf),
             "--frame\r\nContent-Type: image/jpeg\r\nContent-Length: %u\r\n\r\n", (uint32_t)jpg_len);
    res = httpd_resp_send_chunk(req, part_buf, strlen(part_buf));
    res = httpd_resp_send_chunk(req, (const char *)jpg_buf, jpg_len);
    res = httpd_resp_send_chunk(req, "\r\n", 2);

    esp_camera_fb_return(fb);    // Return frame buffer to camera driver
    free(jpg_buf);               // ⚠️ Free manually allocated JPEG buffer

    if (res != ESP_OK) break;   // Break the loop if client disconnects or error occurs
  }

  return res;
}

// 🌍 Start HTTP stream server 
void startCameraServer() {
  httpd_config_t config = HTTPD_DEFAULT_CONFIG();
  config.server_port = 80;

  httpd_uri_t uri = {
    .uri       = "/",
    .method    = HTTP_GET,
    .handler   = stream_handler,
    .user_ctx  = NULL
  };

  if (httpd_start(&stream_httpd, &config) == ESP_OK) {
    httpd_register_uri_handler(stream_httpd, &uri);
    Serial.println("📡 HTTP stream server started");
  } else {
    Serial.println("❌ Failed to start stream server");
  }
}

void setup() {
  Serial.begin(115200);
  Serial.setDebugOutput(true);
  Serial.println("📷 Testing camera init...");

  initCamera();

  // 🚀 Connect to WiFi
  WiFi.begin(ssid, password);
  Serial.print("🔌 Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\n✅ WiFi connected!");
  Serial.print("📶 IP address: ");
  Serial.println(WiFi.localIP());

  startCameraServer();  // Start MJPEG server
  Serial.println("📷 Camera stream ready!");
  Serial.print("👉 Visit: http://");
  Serial.println(WiFi.localIP());
}

void loop() {
  delay(500);  // Nothing to do in loop
}
