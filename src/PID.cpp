//
// Created by Felix on 2025-10-01.
// Rewritten PID control: two independent PIDs + sync correction
//

#include "PID.h"
#include <Arduino.h>
#include <LibRobus.h>


//Honestly, just ask Felix if question

#define SYNC_KP 0.03f
#define SYNC_KP_POS 0.02f



PID motorPID[2];                   // PID controllers for each motor
long lastCountEncoder[2] = {0, 0}; // last encoder readings
long totalCountEncoder[2] = {0, 0}; // total encoder counts (for sync)


static bool rampUp = true;


// Helper clamp
static inline float clampf(float v, float lo, float hi) {
    if (v < lo) return lo;
    if (v > hi) return hi;
    return v;
}

static const float INTEGRAL_MAX = 10.0f; // anti-windup limit


void Advance(float targetSpeed = 1.0f) {
    // Initialize both PIDs (tune these values)
    PIDS_Init(0.30f, 0.04f, 0.03f);

    for (int i = 0; i < 2; i++) {
        motorPID[i].integral = 0.0f;//targetSpeed / motorPID[i].ki * 0.5f; // small prefill
    }

    float currentSpeedRef = 0.0f;
    unsigned long lastUpdate = 0;
    while (true) {
        unsigned long now = millis();

        if (now - lastUpdate >= SampleMs) {

            float accelStep = 0.1f; // small step for smooth acceleration/deceleration

            if (rampUp) {
                currentSpeedRef += accelStep;
                if (currentSpeedRef >= targetSpeed) {
                    currentSpeedRef = targetSpeed;
                    rampUp = false; // start deceleration next
                }
            }
            else {

                currentSpeedRef -= accelStep;
                if (currentSpeedRef <= 0.0f) {
                    currentSpeedRef = 0.0f;
                    rampUp = true; // ready for next run

                    // --- Smooth PID restart handling ---
                    // Don't reset encoder totals (we want straight-line continuity)
                    // Just damp PID state to prevent bursts next cycle
                    for (int i = 0; i < 2; i++) {
                        motorPID[i].integral *= 0.5f;     // decay accumulated error
                        motorPID[i].lastError *= 0.5f;
                        motorPID[i].lastDerivative = 0.0f;
                    }

                    Serial.println("Reached Acceleration End");

                    MOTOR_SetSpeed(0, 0.0f);
                    MOTOR_SetSpeed(1, 0.0f);

                    break;  // exit loop instead of return
                }

            }


            lastUpdate = now;
            PID_ControlMotors(currentSpeedRef);
        }
        // other logic can run here without blocking
    }
}

// Updates both motors with PID + sync correction
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
    control0 = clampf(control0, 0.0f, 1.0f);
    MOTOR_SetSpeed(0, control0);

    // --- Right motor PID + small sync correction ---
    float control1 = PID_Update(&motorPID[1], targetSpeed, measured[1], SampleS);

    // Add small correction based on speed difference
    float syncError = measured[0] - measured[1];
    control1 += SYNC_KP * syncError;

    // Add correction based on **position difference**
    float positionError = (totalCountEncoder[0] - totalCountEncoder[1]) / PPR;
    control1 += SYNC_KP_POS * positionError;

    control1 = clampf(control1, 0.0f, 1.0f);
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

// Track if PID state has been initialized
static bool pidStateInitialized[2] = {false, false};

// Initialize one PID
void PID_Init(PID* _pid, float kp, float ki, float kd, int motorIndex) {
    _pid->kp = kp;
    _pid->ki = ki;
    _pid->kd = kd;

    // Only reset state once per motor
    if (!pidStateInitialized[motorIndex]) {
        _pid->integral = 0.0f;
        _pid->lastError = 0.0f;
        _pid->lastDerivati  ve = 0.0f;
        pidStateInitialized[motorIndex] = true;
    }
}

// Initialize both PIDs
void PIDS_Init(float kp, float ki, float kd) {
    PID_Init(&motorPID[0], kp, ki, kd, 0);
    PID_Init(&motorPID[1], kp, ki, kd, 1);
}


