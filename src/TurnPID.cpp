//
// Created by xilef on 10/6/2025.
//

#include "TurnPID.h"

#include "LibRobus.h"
#include "PID.h"

#define TURN_KP 0.004f
#define TURN_KI 0.0001f
#define TURN_KD 0.0005f

static inline float clampf(float v, float lo, float hi) {
    if (v < lo) return lo;
    if (v > hi) return hi;
    return v;
}

void Turn90(bool turnRight = true) {
    // Initialize both PIDs (separate from forward PIDs)
    PIDS_Init(TURN_KP, TURN_KI, TURN_KD);

    long startLeft  = ENCODER_Read(0);
    long startRight = ENCODER_Read(1);

    // Calculate target encoder difference for 90 degrees
    const float wheelDiameter = 8.0f; // cm
    const float wheelbase = 16.0f;    // cm (adjust for your robot)
    const float wheelCircumference = M_PI * wheelDiameter;
    const float turnDistance = (M_PI * wheelbase) / 4.0f; // quarter turn
    const float rotationsNeeded = turnDistance / wheelCircumference;
    const float pulsesTarget = rotationsNeeded * 3200.0f;

    unsigned long lastUpdate = millis();

    while (true) {
        unsigned long now = millis();
        if (now - lastUpdate < SampleMs) continue;
        lastUpdate = now;

        long leftCount  = ENCODER_Read(0) - startLeft;
        long rightCount = ENCODER_Read(1) - startRight;

        // For right turn, right wheel moves backward, left forward
        float turnError;
        if (turnRight)
            turnError = pulsesTarget - ((leftCount - rightCount) / 2.0f);
        else
            turnError = pulsesTarget - ((rightCount - leftCount) / 2.0f);

        // Convert error to normalized target (just like your advance)
        float errorNorm = turnError / pulsesTarget;
        errorNorm = clampf(errorNorm, -1.0f, 1.0f);

        // PID update for each motor
        float control = PID_Update(&motorPID[0], 0.0f, errorNorm, SampleS);

        // Apply opposite speeds to wheels
        float leftSpeed  =  clampf(control, -1.0f, 1.0f);
        float rightSpeed = -clampf(control, -1.0f, 1.0f);

        MOTOR_SetSpeed(0,  leftSpeed);
        MOTOR_SetSpeed(1,  rightSpeed);

        // Stop condition
        if (fabs(turnError) < 10.0f) { // within 10 pulses
            MOTOR_SetSpeed(0, 0.0f);
            MOTOR_SetSpeed(1, 0.0f);
            Serial.println("Turn 90 complete!");
            break;
        }
    }

}
void TurnLeft() {
    Turn90(false);
}
void TurnRight() {
    Turn90(true);
}