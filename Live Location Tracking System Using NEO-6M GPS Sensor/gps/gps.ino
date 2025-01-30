#include <WiFi.h>
#include <HTTPClient.h>
#include <TinyGPS++.h>

// WiFi Configuration
const char* ssid = "Imon";       // Replace with your WiFi SSID
const char* password = "imon84.."; // Replace with your WiFi password

// Telegram Bot Configuration
const char* botToken = "7700117923:AAF9c0jtH6FB-PLW-AyLtQZ8PWiF9swqby8"; // Replace with your bot token

// List of Chat IDs
const char* chatIDs[] = {"6037168359" , "5119232983"}; // Replace with actual chat IDs
const int numChatIDs = sizeof(chatIDs) / sizeof(chatIDs[0]);

// GPS Configuration
TinyGPSPlus gps;
HardwareSerial GPS_Serial(2); // Use UART2 for GPS communication

// Push Button Configuration
const int buttonPin = 4; // Connect push button to GPIO4
bool buttonPressed = false;

// Timing for Auto-Update
unsigned long lastUpdateTime = 0;
const unsigned long updateInterval = 300000; // 5 minutes in milliseconds 

void IRAM_ATTR handleButtonPress() {
  buttonPressed = true;
}

void setup() {
  // Serial for debugging
  Serial.begin(115200);

  // GPS Serial Setup
  GPS_Serial.begin(9600, SERIAL_8N1, 16, 17); // TX=16, RX=17

  // Push Button Setup
  pinMode(buttonPin, INPUT_PULLUP); // Use internal pull-up resistor
  attachInterrupt(buttonPin, handleButtonPress, FALLING);

  // WiFi Connection
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi!");
}

void sendTelegramMessage(String message) {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;

    for (int i = 0; i < numChatIDs; i++) {
      // Telegram API URL
      String url = "https://api.telegram.org/bot" + String(botToken) + "/sendMessage?chat_id=" + String(chatIDs[i]) + "&text=" + message;

      // Send HTTP GET request
      http.begin(url);
      int httpResponseCode = http.GET();

      if (httpResponseCode > 0) {
        Serial.println("Message sent to chat ID " + String(chatIDs[i]));
        Serial.println(http.getString());
      } else {
        Serial.println("Failed to send message to chat ID " + String(chatIDs[i]) + ". Error: " + String(httpResponseCode));
      }

      http.end();
      delay(1000); // Small delay to avoid Telegram API rate limits
    }
  } else {
    Serial.println("WiFi not connected!");
  }
}

void sendLocation() {
  if (gps.location.isUpdated()) { // Check if a new location is available
    float latitude = gps.location.lat();
    float longitude = gps.location.lng();

    // Format location into Google Maps link
    String mapLink = "https://www.google.com/maps?q=" + String(latitude, 6) + "," + String(longitude, 6);
    Serial.println("Live Location: " + mapLink);

    // Send location to Telegram
    sendTelegramMessage("Live Location: " + mapLink);
  }
}

void loop() {
  // Check for GPS data
  while (GPS_Serial.available() > 0) {
    gps.encode(GPS_Serial.read());
  }

  // Handle Push Button Press
  if (buttonPressed) {
    buttonPressed = false; // Reset button state
    sendLocation();        // Send instant location
  }

  // Handle Auto-Update
  unsigned long currentTime = millis();
  if (currentTime - lastUpdateTime >= updateInterval) {
    lastUpdateTime = currentTime;
    sendLocation(); // Send location every 5 minutes
  }
}
