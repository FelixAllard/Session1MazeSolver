//
// Created by Felix on 2025-10-01.
//

#include "PID.h"

#include <Arduino.h>
#include <LibRobus.h>

///@var Kp Control period
///@var

PID motorPID[2];               // PID for each motor
long lastCountEncoder[2] = {0, 0}; // last encoder readings

void Advance(float targetSpeed) {
    // Initialize PID for both motors
    PIDS_Init(0.2f, 0.05f, 0.01f);



    unsigned long lastUpdate = 0;

    while (true) {
        unsigned long now = millis();
        if (now - lastUpdate >= SampleMs) {
            lastUpdate = now;

            PID_ControlMotors(targetSpeed);
        }

        // other logic can run here without blocking
    }
}

// Updates both motors: master is motor 0, slave is motor 1
void PID_ControlMotors(float targetSpeed) {
    // --- MASTER MOTOR (0) ---
    long count0 = ENCODER_Read(0);
    long diff0  = count0 - lastCountEncoder[0];
    lastCountEncoder[0] = count0;

    float rotations0 = (float)diff0 / PPR;
    float measuredRPS0 = rotations0 / SampleS;
    float measured0 = measuredRPS0 / maxRPS;
    if (measured0 > 1.0f) measured0 = 1.0f;

    float control0 = PID_Update(&motorPID[0], targetSpeed, measured0, SampleS);
    if (control0 < 0.0f) control0 = 0.0f;
    if (control0 > 1.0f) control0 = 1.0f;
    MOTOR_SetSpeed(0, control0);

    // --- SLAVE MOTOR (1) ---
    long count1 = ENCODER_Read(1);
    long diff1  = count1 - lastCountEncoder[1];
    lastCountEncoder[1] = count1;

    float rotations1 = (float)diff1 / PPR;
    float measuredRPS1 = rotations1 / SampleS;
    float measured1 = measuredRPS1 / maxRPS;
    if (measured1 > 1.0f) measured1 = 1.0f;

    // Slave setpoint = master's measured speed (synchronization)
    float control1 = PID_Update(&motorPID[1], measuredRPS0, measuredRPS1, SampleS);
    if (control1 < 0.0f) control1 = 0.0f;
    if (control1 > 1.0f) control1 = 1.0f;
    MOTOR_SetSpeed(1, control1);
}

// PID update function
float PID_Update(PID* Pid, float setpoint, float measured, float dt) {
    float error = setpoint - measured;

    // Proportional
    float P = Pid->kp * error;

    // Integral
    Pid->integral += error * dt;
    float I = Pid->ki * Pid->integral;

    // Derivative
    float derivative = (error - Pid->lastError) / dt;
    float D = Pid->kd * derivative;

    Pid->lastError = error;
    Pid->lastDerivative = derivative;

    return P + I + D;
}

// Initialize one PID
void PID_Init(PID* _pid, float kp, float ki, float kd) {
    _pid->kp = kp;
    _pid->ki = ki;
    _pid->kd = kd;
    _pid->integral = 0.0f;
    _pid->lastError = 0.0f;
    _pid->lastDerivative = 0.0f;
}

// Initialize both PIDs
void PIDS_Init(float kp, float ki, float kd) {
    PID_Init(&motorPID[0], kp, ki, kd);
    PID_Init(&motorPID[1], kp, ki, kd);
}