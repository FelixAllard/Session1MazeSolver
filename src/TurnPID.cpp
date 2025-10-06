//
// Created by xilef on 10/6/2025.
//

#include "TurnPID.h"
#include "LibRobus.h"
#include "PID.h"

#define ENCODER_PPR 3200.0f
#define WHEEL_DIAMETER 7.7f  // cm (calibrated)
#define WHEELBASE 18.0f      // cm (calibrated)
#define TURN_SPEED 0.30f     // constant motor speed
#define STOP_DELAY 50        // ms delay to allow motors to settle after stop

static inline float clampf(float v, float lo, float hi) {
    if (v < lo) return lo;
    if (v > hi) return hi;
    return v;
}

void Turn90(bool turnRight = true) {
    // Reset encoders
    ENCODER_Reset(0);
    ENCODER_Reset(1);

    long startLeft  = 0;
    long startRight = 0;

    // Calculate encoder pulses needed for 90° turn
    const float wheelCirc = M_PI * WHEEL_DIAMETER;
    const float turnDist  = (M_PI * WHEELBASE) / 4.0f;  // quarter circle
    const float rotationsNeeded = turnDist / wheelCirc;
    const float pulsesTarget = rotationsNeeded * ENCODER_PPR;

    Serial.print("Target pulses for 90° turn: ");
    Serial.println(pulsesTarget);

    while (true) {
        // Encoder readings relative to start
        long leftCount  = ENCODER_Read(0) - startLeft;
        long rightCount = ENCODER_Read(1) - startRight;

        // Average distance moved by both wheels
        float avgDist = (fabs(leftCount) + fabs(rightCount)) / 2.0f;

        // Apply constant turn speed (opposite directions for in-place turn)
        MOTOR_SetSpeed(0, turnRight ? TURN_SPEED : -TURN_SPEED);
        MOTOR_SetSpeed(1, turnRight ? -TURN_SPEED : TURN_SPEED);

        // Debug output
        Serial.print("Left: "); Serial.print(leftCount);
        Serial.print(" | Right: "); Serial.print(rightCount);
        Serial.print(" | Avg: "); Serial.print(avgDist);
        Serial.print(" / "); Serial.println(pulsesTarget);

        // Stop condition: when average distance reaches target
        if (avgDist >= pulsesTarget) {
            MOTOR_SetSpeed(0, 0.0f);
            MOTOR_SetSpeed(1, 0.0f);
            delay(STOP_DELAY); // give motors time to physically stop
            Serial.println("Turn 90 complete!");
            break; // exit loop immediately
        }
    }
}

void TurnLeft()  { Turn90(false); }
void TurnRight() { Turn90(true); }
