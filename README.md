# 🎯 **Smart Assistance System for the Visually Impaired** 

## Overview

The **Smart Assistance System for the Visually Impaired** is an innovative IoT-based solution designed to enhance mobility, independence, and safety for visually impaired individuals. This system integrates multiple smart features, including obstacle detection, live location tracking, AI-powered currency detection, a medicine dispenser, and a voice clock.

## 🚀 Features

### 1️⃣ Currency Detection System 💵
- Utilizes **ESP32-CAM** and **MobileNetV2 AI Model** for recognizing Bangladeshi currency notes.
- Captures images, processes them on a **Flask Server**, and provides **audio feedback** using an MP3 module.
- Identifies denominations: **10 Taka, 50 Taka, and 100 Taka**.

### 2️⃣ Medicine Pill Dispenser 💊
- Uses **SG90 Servo Motors** and **DS3231 RTC Module** for scheduled pill dispensing.
- Offers **automated** and **emergency mode** functionalities.
- Stores up to **6 pills**, ensuring timely medication reminders.

### 3️⃣ GPS Live Location Tracking 📍
- Uses **NEO-6M GPS Module** and **ESP32** to send real-time location updates.
- Sends periodic location updates via **Telegram Bot** every **5 minutes**.
- Emergency button allows instant location sharing.

### 4️⃣ Voice Clock ⏰
- Uses **DS3231 RTC Module**, **MP3 Module**, and **Speaker** to announce the current time.
- User-friendly push-button activation for **auditory time alerts**.

### 5️⃣ Obstacle Detection System 🚧
- Uses **HC-SR04 Ultrasonic Sensor** to detect obstacles within a **40 cm range**.
- Alerts users through an **auditory buzzer** to avoid collisions.

## 🛠 Components Used

- **ESP32 DevKit**
- **ESP32-CAM Module**
- **Ultrasonic HC-SR04 Sensor**
- **SG90 Servo Motors**
- **DS3231 RTC Module**
- **Ublox NEO-6M GPS Module**
- **MP3 Module and Speaker**
- **Push Buttons and Buzzers**

## 🏗 System Architecture

1. **Edge Processing**: ESP32 handles sensor data and interacts with peripherals.
2. **AI Processing on Server**: Currency recognition is done via a **Flask server** using **MobileNetV2**.
3. **Cloud Communication**: Uses **Telegram Bot API** for GPS location updates.
4. **Embedded Control**: Uses **RTC Module** and **Servo Motors** for medicine dispensing.

## 🔮 Future Enhancements

- **Integration with AI-powered voice assistants** for better interaction.
- **Support for additional currency denominations**.
- **Smart wearable version** for improved usability.

📢 **Contribute & Support!** 💡 Feel free to **fork**, **star**, and **contribute** to this project!

📩 For inquiries, contact: [mimon2310084@bscse.uiu.ac.bd]
