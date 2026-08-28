#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>

#include "config.h"
#include "state.h"
#include "autonomous.h"
#include "ultrasonic.h"
#include "web.h"

extern WebServer server;

const char htmlPage[] PROGMEM = R"HTML(
<!DOCTYPE html>
<html>
<head>
<meta name="viewport"
      content="width=device-width, initial-scale=1">

<title>ESP32 Robot</title>

<style>
body {
  font-family: Arial, sans-serif;
  text-align: center;
  background: #111;
  color: white;
  margin-top: 60px;
}

button {
  width: 220px;
  height: 80px;
  font-size: 28px;
  margin: 15px;
  border: none;
  border-radius: 15px;
}

#start {
  background: #168a35;
  color: white;
}

#stop {
  background: #b82020;
  color: white;
}

#status {
  margin-top: 30px;
  font-size: 20px;
}
</style>
</head>

<body>

<h1>ESP32 ROBOT</h1>

<button id="start"
        onclick="startRobot()">
  START
</button>

<br>

<button id="stop"
        onclick="stopRobot()">
  STOP
</button>

<div id="status">
  Status: STOPPED
</div>

<script>

function startRobot() {
  fetch('/start')
    .then(() => {
      document.getElementById('status').innerText =
        'Status: RUNNING';
    });
}

function stopRobot() {
  fetch('/stop')
    .then(() => {
      document.getElementById('status').innerText =
        'Status: STOPPED';
    });
}

</script>

</body>
</html>
)HTML";

void handleRoot() {
  server.send_P(200, "text/html", htmlPage);
}

void handleStart() {
  startAutonomous();

  server.send(200, "text/plain", "started");
}

void handleStop() {
  stopAutonomous();

  server.send(200, "text/plain", "stopped");
}

void setupWebServer() {
  server.on("/", handleRoot);
  server.on("/start", handleStart);
  server.on("/stop", handleStop);

  server.begin();

  Serial.println("Web server started");
}

void handleWebClient() {
  server.handleClient();
}
