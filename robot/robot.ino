#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>

#include "config.h"
#include "state.h"
#include "encoders.h"
#include "motors.h"
#include "autonomous.h"
#include "ultrasonic.h"
#include "web.h"

const char* ssid = "ROBOT_ESP32";
const char* password = "roboy1234";

// PID

float Kp = 150.0f;
float Ki = 30.0f;
float Kd = 0.0f;

float RIGHT_MOTOR_SCALE = 0.90f;

// Encoder state

volatile long leftTicks = 0;
volatile long rightTicks = 0;

long prevLeftTicks = 0;
long prevRightTicks = 0;

unsigned long lastControlTime = 0;       // Timing

// Robot state

AutoState autoState = AUTO_IDLE;

float autoTargetHeading = 0;

// PID state

float pidIntegral = 0;
float pidLastError = 0;

// Web server

WebServer server(80);

void setup() {
  Serial.begin(115200);

  // Encoder inputs
  pinMode(L_C1, INPUT_PULLUP);
  pinMode(L_C2, INPUT_PULLUP);

  pinMode(R_C1, INPUT_PULLUP);
  pinMode(R_C2, INPUT_PULLUP);

  attachInterrupt(
      digitalPinToInterrupt(L_C1),
      leftISR, CHANGE);

  attachInterrupt(
      digitalPinToInterrupt(R_C1), rightISR, CHANGE);

  // Motor PWM
  ledcAttach(L_IN1, PWM_FREQ, PWM_RES);

  ledcAttach(L_IN2, PWM_FREQ, PWM_RES);

  ledcAttach(R_IN1, PWM_FREQ, PWM_RES);

  ledcAttach(R_IN2, PWM_FREQ, PWM_RES);

  setMotor(0, 0);

  setupUltrasonic();                      // Ultrasonic sensor

  WiFi.softAP(ssid, password);          // ESP32 creates its own Wi-Fi network.

  Serial.println();
  Serial.println("==============================");
  Serial.println("ESP32 ULTRASONIC ROBOT");
  Serial.println("==============================");

  Serial.print("Wi-Fi SSID: ");
  Serial.println(ssid);

  Serial.print("Wi-Fi Password: ");
  Serial.println(password);

  Serial.print("Robot IP: ");
  Serial.println(WiFi.softAPIP());

  setupWebServer();

  lastControlTime = millis();

  Serial.println("Robot ready.");
}

void loop() {
  handleWebClient();

  if (millis() - lastControlTime >= CONTROL_INTERVAL_MS) {

    lastControlTime = millis();

    updateAutonomous();
  }
}
