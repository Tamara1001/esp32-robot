// robot_proto.ino
// Phase 1 prototype: ESP32 (DevKit) + HC-SR04 + 2 servos + stepper + pot + buzzer/LED,
// controlled from a web joystick over WiFi.
// See docs/PROTOCOL.md and docs/HARDWARE.md.

#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <LittleFS.h>
#include <ESP32Servo.h>

#include "config.h"

AsyncWebServer server(HTTP_PORT);
AsyncWebSocket ws("/ws");

Servo servoPan;
Servo servoTilt;

unsigned long lastSensorMs = 0;
unsigned long lastPotMs = 0;

// ---- Stepper state (non-blocking) ----
const uint8_t STEPPER_PINS[4] = {PIN_STEPPER_IN1, PIN_STEPPER_IN2, PIN_STEPPER_IN3, PIN_STEPPER_IN4};
const uint8_t HALF_STEP_SEQ[8][4] = {
  {1, 0, 0, 0}, {1, 1, 0, 0}, {0, 1, 0, 0}, {0, 1, 1, 0},
  {0, 0, 1, 0}, {0, 0, 1, 1}, {0, 0, 0, 1}, {1, 0, 0, 1},
};
int stepperPosition = 0;   // current position in steps
int stepperTarget = 0;     // target position in steps
uint8_t stepperPhase = 0;
unsigned long lastStepUs = 0;

int lastPotPercent = -100;

// Returns distance in cm, or -1 if no echo / out of range.
float readDistanceCm() {
  digitalWrite(PIN_TRIG, LOW);
  delayMicroseconds(2);
  digitalWrite(PIN_TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(PIN_TRIG, LOW);

  unsigned long duration = pulseIn(PIN_ECHO, HIGH, ECHO_TIMEOUT_US);
  if (duration == 0) return -1.0f;
  float cm = duration * 0.0343f / 2.0f;
  return (cm > MAX_DISTANCE_CM) ? -1.0f : cm;
}

void applyServos(int pan, int tilt) {
  pan = constrain(pan, SERVO_MIN_DEG, SERVO_MAX_DEG);
  tilt = constrain(tilt, SERVO_MIN_DEG, SERVO_MAX_DEG);
  servoPan.write(pan);
  servoTilt.write(tilt);
}

void setStepperTargetDeg(int deg) {
  deg = constrain(deg, 0, 359);
  stepperTarget = (int)((long)deg * STEPPER_STEPS_PER_REV / 360);
}

void releaseStepper() {
  for (uint8_t i = 0; i < 4; i++) digitalWrite(STEPPER_PINS[i], LOW);
}

// Moves one step per call when due; releases coils at rest to avoid heating.
void updateStepper() {
  if (stepperPosition == stepperTarget) {
    releaseStepper();
    return;
  }
  unsigned long nowUs = micros();
  if (nowUs - lastStepUs < STEPPER_STEP_INTERVAL_US) return;
  lastStepUs = nowUs;

  int dir = (stepperTarget > stepperPosition) ? 1 : -1;
  stepperPhase = (stepperPhase + 8 + dir) % 8;
  for (uint8_t i = 0; i < 4; i++) digitalWrite(STEPPER_PINS[i], HALF_STEP_SEQ[stepperPhase][i]);
  stepperPosition += dir;
}

void updateAlert(float cm) {
  bool alert = (cm >= 0 && cm < ALERT_DISTANCE_CM);
  digitalWrite(PIN_BUZZER, alert ? HIGH : LOW);
  digitalWrite(PIN_LED, alert ? HIGH : LOW);
}

void onWsEvent(AsyncWebSocket *s, AsyncWebSocketClient *client, AwsEventType type,
               void *arg, uint8_t *data, size_t len) {
  if (type == WS_EVT_CONNECT) {
    Serial.printf("WS client #%u connected\n", client->id());
  } else if (type == WS_EVT_DISCONNECT) {
    Serial.printf("WS client #%u disconnected\n", client->id());
  } else if (type == WS_EVT_DATA) {
    AwsFrameInfo *info = (AwsFrameInfo *)arg;
    if (!(info->final && info->index == 0 && info->len == len && info->opcode == WS_TEXT)) return;
    char msg[32];
    size_t n = min(len, sizeof(msg) - 1);
    memcpy(msg, data, n);
    msg[n] = '\0';

    int a, b;
    if (sscanf(msg, "S:%d,%d", &a, &b) == 2) {
      applyServos(a, b);
    } else if (sscanf(msg, "T:%d", &a) == 1) {
      setStepperTargetDeg(a);
    }
  }
}

void setup() {
  Serial.begin(115200);

  pinMode(PIN_TRIG, OUTPUT);
  pinMode(PIN_ECHO, INPUT);
  pinMode(PIN_BUZZER, OUTPUT);
  pinMode(PIN_LED, OUTPUT);
  for (uint8_t i = 0; i < 4; i++) pinMode(STEPPER_PINS[i], OUTPUT);
  releaseStepper();
  analogReadResolution(12);

  servoPan.setPeriodHertz(50);
  servoTilt.setPeriodHertz(50);
  servoPan.attach(PIN_SERVO_PAN, SERVO_PULSE_MIN_US, SERVO_PULSE_MAX_US);
  servoTilt.attach(PIN_SERVO_TILT, SERVO_PULSE_MIN_US, SERVO_PULSE_MAX_US);
  applyServos(SERVO_CENTER_DEG, SERVO_CENTER_DEG);

  if (!LittleFS.begin(true)) {
    Serial.println("LittleFS mount failed");
  }

  WiFi.mode(WIFI_AP);
  WiFi.softAP(AP_SSID, AP_PASSWORD);
  Serial.print("AP IP: ");
  Serial.println(WiFi.softAPIP());

  ws.onEvent(onWsEvent);
  server.addHandler(&ws);
  server.serveStatic("/", LittleFS, "/").setDefaultFile("index.html");
  server.begin();
}

void loop() {
  ws.cleanupClients();
  updateStepper();

  unsigned long now = millis();

  if (now - lastSensorMs >= SENSOR_INTERVAL_MS) {
    lastSensorMs = now;
    float cm = readDistanceCm();
    updateAlert(cm);
    if (ws.count() > 0) ws.printfAll("D:%.1f", cm);
  }

  if (now - lastPotMs >= POT_INTERVAL_MS) {
    lastPotMs = now;
    int percent = analogRead(PIN_POT) * 100 / POT_ADC_MAX;
    if (abs(percent - lastPotPercent) >= POT_REPORT_THRESHOLD) {
      lastPotPercent = percent;
      if (ws.count() > 0) ws.printfAll("P:%d", percent);
    }
  }
}
