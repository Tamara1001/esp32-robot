#pragma once

// ---- WiFi (Access Point mode) ----
#define AP_SSID "ESP32-Robot"
#define AP_PASSWORD "robot1234"  // min 8 chars; change it
#define HTTP_PORT 80

// ---- Pins (ESP32 DevKit 38 pins) ----
#define PIN_SERVO_PAN 18
#define PIN_SERVO_TILT 19
#define PIN_TRIG 5
#define PIN_ECHO 4  // HC-SR04 echo is 5V: use a voltage divider (see docs/HARDWARE.md)
#define PIN_POT 34  // ADC1, input only (ADC2 does not work with WiFi)
#define PIN_BUZZER 23  // active buzzer
#define PIN_LED 25
#define PIN_STEPPER_IN1 13  // ULN2003 IN1..IN4
#define PIN_STEPPER_IN2 14
#define PIN_STEPPER_IN3 27
#define PIN_STEPPER_IN4 26

// ---- Servos ----
#define SERVO_MIN_DEG 0
#define SERVO_MAX_DEG 180
#define SERVO_CENTER_DEG 90
#define SERVO_PULSE_MIN_US 500
#define SERVO_PULSE_MAX_US 2400

// ---- Stepper 28BYJ-48 (half-step mode) ----
#define STEPPER_STEPS_PER_REV 4096
#define STEPPER_STEP_INTERVAL_US 2000UL  // lower = faster, too low loses steps

// ---- Ultrasonic sensor ----
#define MAX_DISTANCE_CM 400.0f
#define ECHO_TIMEOUT_US 25000UL
#define SENSOR_INTERVAL_MS 200
#define ALERT_DISTANCE_CM 15.0f  // buzzer + LED when closer than this

// ---- Potentiometer ----
#define POT_ADC_MAX 4095
#define POT_INTERVAL_MS 100
#define POT_REPORT_THRESHOLD 2  // percent change needed to report
