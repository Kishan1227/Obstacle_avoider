#pragma once
#include <Arduino.h>

// ---------------- Wi-Fi ----------------
extern const char* ssid;
extern const char* password;

// ---------------- Wheel encoders ----------------
#define L_C1 16
#define L_C2 17
#define R_C1 18
#define R_C2 19

// ---------------- Motor driver ----------------
#define L_IN1 32
#define L_IN2 33
#define R_IN1 25
#define R_IN2 26

#define PWM_FREQ 5000
#define PWM_RES 8

// ---------------- Robot geometry ----------------
#define WHEEL_CIRCUMFERENCE_MM 100.53f
#define WHEEL_BASE_MM 130.50f
#define LEFT_TICKS_PER_REV 2020.0f
#define RIGHT_TICKS_PER_REV 2000.0f

// ---------------- Motor/PID ----------------
#define BASE_SPEED 200
#define TURN_SPEED 110

#define CONTROL_INTERVAL_MS 50

extern float Kp;
extern float Ki;
extern float Kd;
extern float RIGHT_MOTOR_SCALE;

// ---------------- Ultrasonic HC-SR04 ----------------
// Change these pins if required.
#define ULTRASONIC_TRIG 27
#define ULTRASONIC_ECHO 14

// Robot stops and starts an avoidance turn when an object
// is at or closer than this distance.
#define OBSTACLE_DISTANCE_CM 10.0f

// How far the robot turns when it sees an obstacle.
#define AVOID_TURN_DEGREES 90.0f
