# ESP32 ULTRASONIC OBSTACLE-AVOIDING ROBOT
==========================================

WEB PAGE
--------
Connect your phone to:   

SSID: `ROBOT_ESP32` 
Password: `roboy1234`   

Then open the IP address shown in the Serial Monitor. `192.168.4.1`

The web page contains ONLY:
- START
- STOP

START:
The robot drives forward.
If the ultrasonic sensor detects an object at or closer
than 25 cm, the robot stops, turns right approximately
90 degrees using the wheel encoders, and continues forward.

STOP:
Immediately stops the motors and returns to IDLE.

IMPORTANT:
The HC-SR04 ECHO output can be about 5V.
ESP32 GPIO inputs are 3.3V logic.

Do NOT connect a 5V ECHO signal directly to GPIO 14.

Use a voltage divider.
This reduces approximately 5V to approximately 3.3V.

## IMPORTANT POWER NOTE
--------------------
Do not power the motors from the ESP32 5 V/3.3 V pin.
Use the motor driver's appropriate motor supply.

The ESP32, motor driver and HC-SR04 must share GND.

## BEHAVIOR
--------
1. Press START.
2. Robot drives forward.
3. Ultrasonic sensor measures the front distance.
4. At <= 25 cm, robot stops.
5. Robot turns right about 90 degrees.
6. Robot drives forward again.
7. This repeats until STOP is pressed.

CALIBRATION
-----------
If the robot turns too far or too little, adjust:
`AVOID_TURN_DEGREES`
in `config.h`.

If the obstacle detection distance should change, adjust:
`OBSTACLE_DISTANCE_CM`
in `config.h`.

The supplied encoder constants and motor/PID values were
retained as the starting point from the original project.
