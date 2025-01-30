#include <Wire.h>
#include "RTClib.h"
#include "DFRobotDFPlayerMini.h"

// Define pins
#define speakerPin 16   // Speaker connected to GPIO16
#define buttonPin 17    // Button connected to GPIO17

#if (defined(ARDUINO_AVR_UNO) || defined(ESP8266))
#include <SoftwareSerial.h>
SoftwareSerial softSerial(/*rx =*/19, /*tx =*/5);
#define FPSerial softSerial
#else
#define FPSerial Serial1
#endif

DFRobotDFPlayerMini myDFPlayer;
RTC_DS1307 rtc;

int timeNowHour = 0;
int timeNowMinute = 0;
int timeNowSecond = 0;
unsigned long lastPlayedTime = 0; // Last time the clock spoke
const unsigned long playInterval = 300000; // 5 minutes in milliseconds
bool buttonPressed = false;

void setup() {
  // Serial for debugging
  Serial.begin(115200);

  // Initialize speaker pin
  pinMode(speakerPin, OUTPUT);

  // Initialize button
  pinMode(buttonPin, INPUT_PULLUP);

  // Initialize DFPlayer Mini
  #if (defined ESP32)
    FPSerial.begin(9600, SERIAL_8N1, /*rx =*/19, /*tx =*/18);
  #else
    FPSerial.begin(9600);
  #endif

  Serial.println(F("Initializing DFPlayer Mini..."));
  if (!myDFPlayer.begin(FPSerial, /*isACK=*/true, /*doReset=*/true)) {
    Serial.println(F("Unable to initialize DFPlayer Mini. Check connections and SD card!"));
    while (true);
  }
  myDFPlayer.setTimeOut(500);
  myDFPlayer.volume(30);
  myDFPlayer.EQ(DFPLAYER_EQ_NORMAL);
  myDFPlayer.outputDevice(DFPLAYER_DEVICE_SD);
  Serial.println(F("DFPlayer Mini online."));

  // Initialize RTC
  if (!rtc.begin()) {
    Serial.println("Couldn't find RTC");
    while (true);
  }
}

void loop() {
  // Handle clock functionality
  handleClock();
}

void handleClock() {
  DateTime now = rtc.now();
  timeNowHour = now.hour();
  timeNowMinute = now.minute();
  timeNowSecond = now.second();

  String meridiem = "AM";
  int displayHour = timeNowHour;

  // Convert to 12-hour format
  if (displayHour == 0) {
    displayHour = 12; // Midnight
  } else if (displayHour == 12) {
    meridiem = "PM"; // Noon
  } else if (displayHour > 12) {
    displayHour -= 12;
    meridiem = "PM";
  }

  Serial.print("Time: ");
  Serial.print(displayHour);
  Serial.print(":");
  Serial.print(timeNowMinute);
  Serial.print(":");
  Serial.print(timeNowSecond);
  Serial.print(" ");
  Serial.println(meridiem);

  // Check if the button is pressed
  if (digitalRead(buttonPin) == LOW && !buttonPressed) {
    buttonPressed = true;
    speakTime(displayHour, timeNowMinute, meridiem);  // Play the time when the button is pressed
    lastPlayedTime = millis(); // Update the last played time
  }
  
  // Reset button press if it's released
  if (digitalRead(buttonPin) == HIGH && buttonPressed) {
    buttonPressed = false;
  }

  // Play voice after 5 minutes from last play
  if (millis() - lastPlayedTime >= playInterval) {
    speakTime(displayHour, timeNowMinute, meridiem);
    lastPlayedTime = millis();  // Update the last played time
  }
}

void speakTime(int hours, int minutes, String meridiem) {
  Serial.print("Speaking Time: ");
  Serial.print(hours);
  Serial.print(":");
  Serial.print(minutes);
  Serial.print(" ");
  Serial.println(meridiem);

  // Speaking the hour
  myDFPlayer.playFolder(1, hours); // Folder 1 contains hour audio files
  delay(2000);

  // Speaking the minutes
  myDFPlayer.playFolder(2, minutes); // Folder 2 contains minute audio files
  delay(2000);
}
