#include <Wire.h>

#define echoPin 2               // Echo pin of HC-SR04
#define trigPin 4               // Trig pin of HC-SR04
#define speakerPin 16            // Speaker connected to GPIO5

long duration, distance;

void setup() {
  Serial.begin(9600);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(speakerPin, OUTPUT);
}

void loop() {
  // Trigger the ultrasonic sensor
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // Measure the echo duration
  duration = pulseIn(echoPin, HIGH);

  // Calculate distance (in cm)
  distance = duration / 58.2;
  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");

  // Play "beep-beep" sound if distance is less than 35 cm
  if (distance < 35) {
    // Beep 1
    tone(speakerPin, 500); // 1kHz tone
    delay(200);            // 200ms beep
    noTone(speakerPin);    // Stop the tone
    delay(200);            // 200ms silence

    // Beep 2
    tone(speakerPin, 500); // 1kHz tone
    delay(200);            // 200ms beep
    noTone(speakerPin);    // Stop the tone
  } else {
    noTone(speakerPin); // Ensure the speaker is silent
  }

  delay(200); // Wait 1 second before rechecking
}
