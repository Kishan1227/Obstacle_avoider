#pragma once
#include <Arduino.h>

enum AutoState {
  AUTO_IDLE,
  AUTO_DRIVE,
  AUTO_AVOID_TURN,
  AUTO_DONE
};

extern volatile long leftTicks;
extern volatile long rightTicks;

extern long prevLeftTicks;
extern long prevRightTicks;

extern unsigned long lastControlTime;

extern AutoState autoState;

extern float autoTargetHeading;

extern float pidIntegral;
extern float pidLastError;
