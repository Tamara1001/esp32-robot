// robot_proto.ino
// Phase 1 prototype: ESP32 (DevKit) + HC-SR04 + 2 servos, controlled from a web joystick over WiFi.
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
unsigned long lastCommandMs = 0;

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

    int pan, tilt;
    if (sscanf(msg, "S:%d,%d", &pan, &tilt) == 2) {
      applyServos(pan, tilt);
      lastCommandMs = millis();
    }
  }
}

void setup() {
  Serial.begin(115200);

  pinMode(PIN_TRIG, OUTPUT);
  pinMode(PIN_ECHO, INPUT);

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

  unsigned long now = millis();
  if (now - lastSensorMs >= SENSOR_INTERVAL_MS) {
    lastSensorMs = now;
    if (ws.count() > 0) {
      ws.printfAll("D:%.1f", readDistanceCm());
    }
  }
}
