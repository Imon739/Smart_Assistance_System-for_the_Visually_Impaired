#include <Servo.h>    // Servo library
#include <Wire.h>     // I2C communication for RTC
#include "RTClib.h"   // RTC library

// Servo objects
Servo servo1; // Push Button Controlled
Servo servo2; // RTC Controlled

// RTC object
RTC_DS3231 rtc;

// Pin definitions
#define servo1Pin 9   // Servo 1 (Push Button)
#define servo2Pin 10  // Servo 2 (RTC Controlled)
#define buttonPin 2   // Push Button

// Variables for button-controlled servo
bool buttonState = false;
bool lastButtonState = false;

// Variables for RTC-controlled servo
int triggerHour1 = 17;    // First trigger hour
int triggerMinute1 = 10;  // First trigger minute
int triggerHour2 = 17;    // Second trigger hour
int triggerMinute2 = 12;  // Second trigger minute
int triggerHour3 = 17;    // Third trigger hour
int triggerMinute3 = 13;  // Third trigger minute
int COUNT1 = 1;
int COUNT2 = 1;

void setup() {
  Serial.begin(9600);

  // Attach servos
  servo1.attach(servo1Pin);
  servo2.attach(servo2Pin);

  // Initialize servo positions
  servo1.write(0);
  servo2.write(0);

  // Initialize button pin
  pinMode(buttonPin, INPUT_PULLUP); // Use internal pull-up resistor

  // Initialize RTC
  if (!rtc.begin()) {
    Serial.println("Couldn't find RTC!");
    while (1); // Halt the program if RTC is not found
  }

  // Uncomment this line to set RTC time to the compile time
  // rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
}

void loop() {
  // Handle the push button-controlled servo
  handleButtonServo();

  // Handle the RTC-controlled servo
  handleTimeServo();
}

void handleButtonServo() {
  // Read the button state
  buttonState = digitalRead(buttonPin) == LOW; // Button is active LOW

  // Check if the button state has changed
  if (buttonState && !lastButtonState) {
    Serial.println("Button pressed, moving Servo 1.");
    servo1.write(60 * COUNT1); // Move Servo 1 in increments of 60 degrees
    delay(1000);               // Hold for 1 second
    servo1.write(0);           // Reset to 0 degrees
    delay(1000);
    COUNT1++;
    if (COUNT1 > 3) COUNT1 = 1; // Reset after 180 degrees
  }

  // Update last button state
  lastButtonState = buttonState;
}

void handleTimeServo() {
  // Get the current time
  DateTime now = rtc.now();

  // Trigger Servo 2 based on specific times
  if ((now.hour() == triggerHour1 && now.minute() == triggerMinute1 && now.second() == 0) ||
      (now.hour() == triggerHour2 && now.minute() == triggerMinute2 && now.second() == 0) ||
      (now.hour() == triggerHour3 && now.minute() == triggerMinute3 && now.second() == 0)) {
    Serial.println("Time event triggered, moving Servo 2.");
    servo2.write(60 * COUNT2); // Move Servo 2 in increments of 60 degrees
    delay(1000);               // Hold for 1 second
    servo2.write(0);           // Reset to 0 degrees
    delay(1000);
    COUNT2++;
    if (COUNT2 > 3) COUNT2 = 1; // Reset after 180 degrees
  }
}
