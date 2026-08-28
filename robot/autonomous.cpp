#include <Arduino.h>
#include <math.h>

#include "config.h"
#include "state.h"
#include "encoders.h"
#include "motors.h"
#include "ultrasonic.h"
#include "autonomous.h"

static float normalizeAngle(float angle) {

  while (angle > 180.0f)
    angle -= 360.0f;

  while (angle < -180.0f)
    angle += 360.0f;

  return angle;
}

// START ROBOT

void startAutonomous() {

  autoState = AUTO_DRIVE;

  resetPID();

  Serial.println("AUTONOMOUS START");
}

// STOP ROBOT

void stopAutonomous() {

  setMotor(0, 0);

  resetPID();

  autoState = AUTO_IDLE;

  Serial.println("ROBOT STOPPED");
}

// START A TURN

static void startAvoidTurn(float degrees) {

  float currentHeading = getHeadingDeg();

  autoTargetHeading = currentHeading + degrees;

  resetPID();

  autoState = AUTO_AVOID_TURN;

  Serial.print("Turning ");
  Serial.print(degrees);
  Serial.println(" degrees");
}

// CHECK WHETHER ROBOT CAN CONTINUE

static bool pathBlocked() {

  float distance = readDistanceCM();

  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");

  if (distance <= 0.0f) {

    // No valid echo.
    // Treat it as no obstacle.
    return false;
  }

  return distance <= OBSTACLE_DISTANCE_CM;
}

// FORWARD MOVEMENT

static void updateDrive() {

  // Check obstacle

  if (pathBlocked()) {

    setMotor(0, 0);

    delay(150);

    Serial.println("OBSTACLE DETECTED");
    Serial.println("STEP 1: TURN 90 RIGHT");

    // First attempt:
    // 90 degrees RIGHT
    startAvoidTurn(90.0f);

    return;
  }

  // Normal forward movement

  int speedL = BASE_SPEED;
  int speedR = BASE_SPEED;

  applyStraightPID(speedL, speedR);

  setMotor(speedL, speedR);
}

// TURN CONTROL

static void updateAvoidTurn() {

  float currentHeading = getHeadingDeg();

  float error = normalizeAngle(autoTargetHeading - currentHeading);

  // Turn completed

  if (fabsf(error) <= 2.0f) {

    setMotor(0, 0);

    delay(200);

    resetPID();

    // Check the new direction

    float distance = readDistanceCM();

    Serial.print("After turn distance: ");
    Serial.print(distance);
    Serial.println(" cm");


    bool blocked = false;

    if (distance > 0.0f && distance <= OBSTACLE_DISTANCE_CM) {

      blocked = true;
    }

    // FIRST TURN FAILED
    // 90 RIGHT -> STILL BLOCKED

    if (blocked) {

      Serial.println("STILL BLOCKED AFTER 90 RIGHT");

      Serial.println("STEP 2: TURN 180 LEFT");

      // From current position:
      // turn 180 degrees LEFT.
      startAvoidTurn(-180.0f);

      return;
    }

    // PATH IS CLEAR

    Serial.println("PATH CLEAR - CONTINUE FORWARD");

    autoState = AUTO_DRIVE;

    return;
  }

  // Slow down near target angle

  int turnSpeed = TURN_SPEED;

  if (fabsf(error) < 20.0f)
    turnSpeed = 75;

  // Turn direction

  if (error > 0) {
    setMotor(-turnSpeed, turnSpeed);                      // RIGHT
  } else {
    setMotor(turnSpeed, -turnSpeed);                      // LEFT
  }
}

// MAIN AUTONOMOUS CONTROL

void updateAutonomous() {

  if (autoState == AUTO_DRIVE) {

    updateDrive();
  }

  else if (autoState == AUTO_AVOID_TURN) {

    updateAvoidTurn();
  }

  else if (autoState == AUTO_IDLE || autoState == AUTO_DONE) {

    setMotor(0, 0);
  }
}