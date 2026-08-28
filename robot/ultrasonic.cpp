#include <Arduino.h>
#include "config.h"
#include "ultrasonic.h"

void setupUltrasonic() {
  pinMode(ULTRASONIC_TRIG, OUTPUT);
  pinMode(ULTRASONIC_ECHO, INPUT);

  digitalWrite(ULTRASONIC_TRIG, LOW);
}

float readDistanceCM() {
  // Trigger a 10 us ultrasonic pulse.
  digitalWrite(ULTRASONIC_TRIG, LOW);
  delayMicroseconds(2);

  digitalWrite(ULTRASONIC_TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(ULTRASONIC_TRIG, LOW);

  // Timeout prevents the robot from waiting too long.
  unsigned long duration = pulseIn(ULTRASONIC_ECHO, HIGH, 30000UL);

  // 0 means no valid echo was received.
  if (duration == 0)
    return 0.0f;

  // Speed of sound conversion:
  // distance(cm) = time(us) / 58
  return (float)duration / 58.0f;
}

bool obstacleDetected() {
  float distance = readDistanceCM();

  // No echo is treated as "no detected obstacle".
  if (distance <= 0.0f)
    return false;

  return distance <= OBSTACLE_DISTANCE_CM;
}
