#include "esp_camera.h"
#include <WiFi.h>
#include <HTTPClient.h>
#include "DFRobotDFPlayerMini.h"

// Wi-Fi credentials
const char* ssid = "XXXXXXXXXX"; // Replace with your Wi-Fi SSID
const char* password = ".........."; // Replace with your Wi-Fi password

// Flask server URL
const char* serverUrl = "http://192.168.49.198:5000/predict"; // Replace with your Flask server IP and port

// AI-Thinker ESP32-CAM Pin Mapping
#define PWDN_GPIO_NUM    32
#define RESET_GPIO_NUM   -1
#define XCLK_GPIO_NUM    0
#define SIOD_GPIO_NUM    26
#define SIOC_GPIO_NUM    27
#define Y9_GPIO_NUM      35
#define Y8_GPIO_NUM      34
#define Y7_GPIO_NUM      39
#define Y6_GPIO_NUM      36
#define Y5_GPIO_NUM      21
#define Y4_GPIO_NUM      19
#define Y3_GPIO_NUM      18
#define Y2_GPIO_NUM      5
#define VSYNC_GPIO_NUM   25
#define HREF_GPIO_NUM    23
#define PCLK_GPIO_NUM    22

// MP3 module setup
HardwareSerial mp3Serial(1); // Use UART1
DFRobotDFPlayerMini mp3;

void playAudio(String currency) {
  // Map currency name to audio file index
  int fileIndex = -1;
  if (currency == "100 Taka") fileIndex = 1; // Maps to 0001.mp3
  else if (currency == "50 Taka") fileIndex = 2; // Maps to 0002.mp3
  else if (currency == "10 Taka") fileIndex = 3; // Maps to 0003.mp3

  if (fileIndex > 0) {
    mp3.play(fileIndex); // Play the corresponding audio file
    Serial.printf("Playing audio file %d for %s\n", fileIndex, currency.c_str());
  } else {
    Serial.println("Unrecognized currency!");
  }
}

void setup() {
  Serial.begin(115200);
  mp3Serial.begin(9600, SERIAL_8N1, 13, 14); // UART1: RX=GPIO13, TX=GPIO14

  // Initialize DFPlayer
  if (!mp3.begin(mp3Serial)) {
    Serial.println("DFPlayer Mini initialization failed!");
    while (true);
  }
  mp3.setTimeOut(500);
  mp3.volume(25); // Set volume (0-30)
  mp3.EQ(DFPLAYER_EQ_NORMAL);
  mp3.outputDevice(DFPLAYER_DEVICE_SD);

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nConnected to Wi-Fi");

  // Camera configuration
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
  config.xclk_freq_hz = 20000000;
  config.frame_size = FRAMESIZE_QVGA;
  config.pixel_format = PIXFORMAT_JPEG;
  config.grab_mode = CAMERA_GRAB_WHEN_EMPTY;
  config.fb_location = CAMERA_FB_IN_PSRAM;
  config.jpeg_quality = 12;
  config.fb_count = 1;

  // Initialize the camera
  esp_err_t err = esp_camera_init(&config);
  if (err != ESP_OK) {
    Serial.printf("Camera init failed with error 0x%x\n", err);
    while (true);
  }
  Serial.println("Camera initialized successfully");
}

void loop() {
  // Capture an image
  camera_fb_t *fb = esp_camera_fb_get();
  if (!fb) {
    Serial.println("Failed to capture image");
    return;
  }

  Serial.printf("Image captured! Size: %u bytes\n", fb->len);

  // Send image to the Flask server
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    http.begin(serverUrl);
    http.addHeader("Content-Type", "image/jpeg");

    int httpResponseCode = http.POST(fb->buf, fb->len); // Send the image buffer
    if (httpResponseCode > 0) {
      String response = http.getString(); // Read server response
      Serial.println("Server response: " + response);

      // Play corresponding audio file
      playAudio(response);
    } else {
      Serial.printf("HTTP POST failed: %s\n", http.errorToString(httpResponseCode).c_str());
    }

    http.end();
  } else {
    Serial.println("Wi-Fi disconnected!");
  }

  // Free the frame buffer
  esp_camera_fb_return(fb);

  delay(5000); // Wait 5 seconds before capturing the next image
}
