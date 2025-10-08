
#include "TurnPID.h"
#include "LibRobus.h"
#include "PID.h"


#define ENCODER_PPR 3200.0f
#define WHEEL_DIAMETER 7.7f   // cm
#define WHEELBASE 18.0f       // cm
#define TURN_SPEED 0.30f      // constant turning speed
#define STOP_DELAY 50         // ms delay after stop

// Slight bias to correct motor imbalance
#define LEFT_MOTOR_BIAS 0.97f   // reduce if left pushes harder
#define RIGHT_MOTOR_BIAS 1.00f  // keep right as reference

static inline float clampf(float v, float lo, float hi) {
    if (v < lo) return lo;
    if (v > hi) return hi;
    return v;
}

void Turn90(bool turnRight = true) {
    // Reset encoders
    ENCODER_Reset(0);
    ENCODER_Reset(1);

    // Calculate pulses needed for a 90° turn
    const float wheelCirc = M_PI * WHEEL_DIAMETER;
    const float turnDist  = (M_PI * WHEELBASE) / 4.0f;  // quarter circle
    const float rotationsNeeded = turnDist / wheelCirc;
    const float pulsesTarget = rotationsNeeded * ENCODER_PPR;

    while (true) {
        long leftCount  = ENCODER_Read(0);
        long rightCount = ENCODER_Read(1);

        // Average distance moved
        float avgDist = (fabs(leftCount) + fabs(rightCount)) / 2.0f;

        // Apply constant speed with small bias correction
        float leftSpeed  = (turnRight ?  TURN_SPEED : -TURN_SPEED) * LEFT_MOTOR_BIAS;
        float rightSpeed = (turnRight ? -TURN_SPEED :  TURN_SPEED) * RIGHT_MOTOR_BIAS;

        MOTOR_SetSpeed(0, leftSpeed);
        MOTOR_SetSpeed(1, rightSpeed);

        // Stop when target reached
        if (avgDist >= pulsesTarget) {
            MOTOR_SetSpeed(0, 0.0f);
            MOTOR_SetSpeed(1, 0.0f);
            delay(STOP_DELAY);
            break;
        }
    }
}

void TurnLeft()  { Turn90(false); }
void TurnRight() { Turn90(true); }
