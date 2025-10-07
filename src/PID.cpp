//
// Created by Felix on 2025-10-01.
// Rewritten PID control: two independent PIDs + sync correction
//

#include "PID.h"
#include <Arduino.h>
#include <LibRobus.h>


//Honestly, just ask Felix if question

#define SYNC_KP 0.05f
#define SYNC_KP_POS 0.02f




PID motorPID[2];                   // PID controllers for each motor
long lastCountEncoder[2] = {0, 0}; // last encoder readings
long totalCountEncoder[2] = {0, 0}; // total encoder counts (for sync)


static bool rampUp = true;
static const float INTEGRAL_MAX = 10.0f; // anti-windup limit


// Helper clamp
static inline float clampf(float v, float lo, float hi) {
    if (v < lo) return lo;
    if (v > hi) return hi;
    return v;
}

// Track if PID state has been initialized
static bool pidStateInitialized[2] = {false, false};

void ResetPIDState() {
    for (int i = 0; i < 2; i++) {
        motorPID[i].integral = 0.0f;
        motorPID[i].lastError = 0.0f;
        motorPID[i].lastDerivative = 0.0f;
    }
}

void Advance(float motorFraction) {
    // motorFraction is now 0–1 (0 = stopped, 1 = full speed)
    motorFraction = clampf(motorFraction, 0.0f, 1.0f);

    // Initialize both PIDs
    PIDS_Init(0.2f, 0.07f, 0.04f); // inside AdvanceDistance()


    // Reset integrals
    for (int i = 0; i < 2; i++) {
        motorPID[i].integral = 0.0f;
    }

    float currentSpeedRef = 0.0f; // ramping variable
    unsigned long lastUpdate = 0;
    const float accelStep = 0.2f; // fraction per loop

    while (true) {
        unsigned long now = millis();
        if (now - lastUpdate >= SampleMs) {

            // --- Ramp up / down ---
            if (rampUp) {
                currentSpeedRef += accelStep;
                if (currentSpeedRef >= motorFraction) {
                    currentSpeedRef = motorFraction;
                    rampUp = false; // start deceleration next
                }
            } else {
                currentSpeedRef -= accelStep;
                if (currentSpeedRef <= 0.0f) {
                    currentSpeedRef = 0.0f;
                    rampUp = true;

                    // Smooth PID damping
                    for (int i = 0; i < 2; i++) {
                        motorPID[i].integral *= 0.5f;
                        motorPID[i].lastError *= 0.5f;
                        motorPID[i].lastDerivative = 0.0f;
                    }

                    MOTOR_SetSpeed(0, 0.0f);
                    MOTOR_SetSpeed(1, 0.0f);
                    Serial.println("Reached Acceleration End");
                    break;
                }
            }

            lastUpdate = now;

            // --- Update motors with fraction directly ---
            PID_ControlMotors(currentSpeedRef);
        }
    }
}

// Moves the robot a specific distance in meters
// fractionSpeed: 0.0–1.0 (normalized motor speed fraction)





// Updates both motors with PID + sync correction
void AdvanceDistance(float distanceMeters, float fractionSpeed) {
    // Clamp target speed
    fractionSpeed = clampf(fractionSpeed, 0.0f, 1.0f);

    // Convert distance to encoder pulses
    const float wheelRadius = 0.0385f;
    long targetPulses = (long)((distanceMeters / (2.0f * 3.14159f * wheelRadius)) * PPR);

    // Reset encoders and PID
    ENCODER_Reset(0);
    ENCODER_Reset(1);
    lastCountEncoder[0] = 0;
    lastCountEncoder[1] = 0;
    totalCountEncoder[0] = 0;
    totalCountEncoder[1] = 0;
    ResetPIDState();

    // Slightly smoother PID gains for direct control
    PIDS_Init(0.30f, 0.03f, 0.03f);

    unsigned long lastUpdate = millis();
    float currentSpeedRef = 0.0f;      // internal target used by PID
    const float step = 0.1f;          // how fast we "snap" toward target (not full accel)

    while (true) {
        unsigned long now = millis();
        if (now - lastUpdate >= SampleMs) {
            lastUpdate = now;

            // --- Gradually move target toward desired speed ---
            if (currentSpeedRef < fractionSpeed) {
                currentSpeedRef += step;
                if (currentSpeedRef > fractionSpeed)
                    currentSpeedRef = fractionSpeed;
            }

            // --- PID control ---
            PID_ControlMotors(currentSpeedRef);

            // --- Check distance ---
            long avgPulses = (totalCountEncoder[0] + totalCountEncoder[1]) / 2;
            if (avgPulses >= targetPulses)
                break;
        }
    }

    // Stop
    MOTOR_SetSpeed(0, 0.0f);
    MOTOR_SetSpeed(1, 0.0f);
}



void PID_ControlMotors(float targetSpeed) {
    float measured[2];
    long diff[2];

    // --- Read encoders and calculate normalized speed ---
    for (int i = 0; i < 2; i++) {
        long count = ENCODER_Read(i);
        diff[i] = count - lastCountEncoder[i];
        lastCountEncoder[i] = count;

        totalCountEncoder[i] += diff[i]; // accumulate for possible future use

        float rotations = (float)diff[i] / PPR;
        float measuredRPS = rotations / SampleS;
        measured[i] = measuredRPS / maxRPS;
        measured[i] = clampf(measured[i], 0.0f, 1.0f);
    }

    // --- Left motor PID ---
    float control0 = PID_Update(&motorPID[0], targetSpeed, measured[0], SampleS);

    // --- Right motor PID ---
    float control1 = PID_Update(&motorPID[1], targetSpeed, measured[1], SampleS);

    // --- Sync corrections ---
    float syncError = measured[0] - measured[1];  // speed difference
    control1 += SYNC_KP * syncError;

    float positionError = (totalCountEncoder[0] - totalCountEncoder[1]) / PPR; // distance diff
    control1 += SYNC_KP_POS * positionError;

    // --- Static motor bias correction (hardware imbalance) ---
    const float motorBias[2] = {1.00f, 1.017f}; // right motor needs ~4% more speed //TODO fight right ratio
    control0 *= motorBias[0];
    control1 *= motorBias[1];

    // --- Clamp and send to motors ---
    control0 = clampf(control0, 0.0f, 1.0f);
    control1 = clampf(control1, 0.0f, 1.0f);

    MOTOR_SetSpeed(0, control0);
    MOTOR_SetSpeed(1, control1);
}

// PID update function (with anti-windup clamp)
float PID_Update(PID* Pid, float setpoint, float measured, float dt) {
    float error = setpoint - measured;

    // Proportional
    float P = Pid->kp * error;

    // Integral
    Pid->integral += error * dt;
    if (Pid->integral > INTEGRAL_MAX) Pid->integral = INTEGRAL_MAX;
    if (Pid->integral < -INTEGRAL_MAX) Pid->integral = -INTEGRAL_MAX;
    float I = Pid->ki * Pid->integral;

    // Derivative
    float derivative = 0.0f;
    if (dt > 0.0f) {
        derivative = (error - Pid->lastError) / dt;
    }
    float D = Pid->kd * derivative;

    // Save state
    Pid->lastError = error;
    Pid->lastDerivative = derivative;

    return P + I + D;
}




// Initialize one PID
void PID_Init(PID* _pid, float kp, float ki, float kd, int motorIndex) {
    _pid->kp = kp;
    _pid->ki = ki;
    _pid->kd = kd;

    // Only reset state once per motor
    if (!pidStateInitialized[motorIndex]) {
        _pid->integral = 0.0f;
        _pid->lastError = 0.0f;
        _pid->lastDerivative = 0.0f;
        pidStateInitialized[motorIndex] = true;
    }
}

// Initialize both PIDs
void PIDS_Init(float kp, float ki, float kd) {
    PID_Init(&motorPID[0], kp, ki, kd, 0);
    PID_Init(&motorPID[1], kp, ki, kd, 1);
}