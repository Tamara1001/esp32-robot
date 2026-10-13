#pragma once

// ---- WiFi (Access Point mode) ----
#define AP_SSID "ESP32-Robot"
#define AP_PASSWORD "robot1234"  // min 8 chars; change it
#define HTTP_PORT 80

// ---- Pins (ESP32 DevKit) ----
#define PIN_SERVO_PAN 18
#define PIN_SERVO_TILT 19
#define PIN_TRIG 5
#define PIN_ECHO 4  // HC-SR04 echo is 5V: use a voltage divider (see docs/HARDWARE.md)

// ---- Servos ----
#define SERVO_MIN_DEG 0
#define SERVO_MAX_DEG 180
#define SERVO_CENTER_DEG 90
#define SERVO_PULSE_MIN_US 500
#define SERVO_PULSE_MAX_US 2400

// ---- Ultrasonic sensor ----
#define MAX_DISTANCE_CM 400.0f
#define ECHO_TIMEOUT_US 25000UL
#define SENSOR_INTERVAL_MS 200
