//
// Created by xilef on 10/6/2025.
//

#include "TurnPID.h"

#include "LibRobus.h"
#include "PID.h"



#define TURN_KP 0.05f
#define TURN_KI 0.001f
#define TURN_KD 0.002f

#define STOP_THRESHOLD 50   // pulses tolerance to stop
#define ENCODER_PPR 3200.0f
#define WHEEL_DIAMETER 8.0f  // cm
#define WHEELBASE 16.0f      // cm
#define SAMPLE_MS 10          // PID update interval in milliseconds

static inline float clampf(float v, float lo, float hi) {
    if (v < lo) return lo;
    if (v > hi) return hi;
    return v;
}

float ffabs(float x) {
    if (x < 0.0f) {
        return -x;
    } else {
        return x;
    }
}
void Turn90(bool turnRight = true) {
    // Initialize separate PIDs for left and right
    PIDS_Init(TURN_KP, TURN_KI, TURN_KD);

    long startLeft  = ENCODER_Read(0);
    long startRight = ENCODER_Read(1);

    // Calculate pulses needed for 90° turn
    const float wheelCirc = M_PI * WHEEL_DIAMETER;
    const float turnDist  = (M_PI * WHEELBASE) / 4.0f;  // quarter turn
    const float rotationsNeeded = turnDist / wheelCirc;
    const float pulsesTarget = rotationsNeeded * ENCODER_PPR;

    unsigned long lastUpdate = millis();

    while (true) {
        unsigned long now = millis();
        if (now - lastUpdate < SAMPLE_MS) continue;
        lastUpdate = now;

        // Read encoder differences
        long leftCount  = ENCODER_Read(0) - startLeft;
        long rightCount = ENCODER_Read(1) - startRight;

        // Compute turn error: average wheel movement
        float turnError = pulsesTarget - (ffabs(leftCount) + ffabs(rightCount)) / 2.0f;

        // Normalize error [-1, 1]
        float errorNorm = clampf(turnError / pulsesTarget, -1.0f, 1.0f);

        // PID update (same controller for both wheels)
        float control = PID_Update(&motorPID[0], 0.0f, errorNorm, SAMPLE_MS / 1000.0f);

        // Apply speeds: opposite for turning
        float leftSpeed  = clampf(control, -1.0f, 1.0f);
        float rightSpeed = -clampf(control, -1.0f, 1.0f);

        if (!turnRight) {  // swap directions for left turn
            leftSpeed  = -leftSpeed;
            rightSpeed = -rightSpeed;
        }

        MOTOR_SetSpeed(0, leftSpeed);
        MOTOR_SetSpeed(1, rightSpeed);

        Serial.print(leftSpeed);
        Serial.print(rightSpeed);

        // Debug print
        Serial.print("Left: "); Serial.print(leftCount);
        Serial.print(" Right: "); Serial.print(rightCount);
        Serial.print(" Error: "); Serial.println(turnError);

        // Stop condition
        if (fabs(turnError) < STOP_THRESHOLD) {
            MOTOR_SetSpeed(0, 0.0f);
            MOTOR_SetSpeed(1, 0.0f);
            Serial.println("Turn 90 complete!");
            break;
        }
    }
}

void TurnLeft()  { Turn90(false); }
void TurnRight() { Turn90(true); }
