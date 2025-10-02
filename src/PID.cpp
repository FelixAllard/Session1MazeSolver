//
// Created by Felix on 2025-10-01.
//

#include "PID.h"

#include <Arduino.h>
#include <LibRobus.h>

///@var Kp Control period
///@var

struct PID motorPID[2];  // motorPID[0] for motor 0, motorPID[1] for motor 1

long lastCountEncoder[2] = {0, 0};


void Advance(float targetSpeed) {

    PIDS_Init(0.2f, 0.1f, 0.01f);

    unsigned long lastUpdate = 0;

    while (true) {
        unsigned long now = millis();
        if (now - lastUpdate >= SampleMs) {
            lastUpdate = now;

            PID_ControlMotor(0, targetSpeed, SampleS);
            PID_ControlMotor(1, targetSpeed, SampleS);
        }

        // other robot logic can run here while motors keep spinning
    }
    /*// Initialize both motor PIDs
    PIDS_Init(0.6f, 0.3f, 0.02f);

    // Main PID loop
    while (true) {
        delay(SampleMs);

        // Update both motors
        PID_ControlMotor(0, targetSpeed, SampleS);
        PID_ControlMotor(1, targetSpeed, SampleS);
    }*/
}
float PID_Update(struct PID *Pid, float setpoint, float measured, float dt) {
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

    // Output
    return P + I + D;
}
void PID_ControlMotor(int motor, float setpoint, float dt) {
    long count = ENCODER_Read(motor);   // absolute count
    long diff = count - lastCountEncoder[motor];
    lastCountEncoder[motor] = count;

    float rotations = (float)diff / PPR;
    float measuredRPS = rotations / dt;
    float measured = measuredRPS / maxRPS;
    if (measured > 1.0f) measured = 1.0f;

    float control = PID_Update(&motorPID[motor], setpoint, measured, dt);

    if (control < 0.0f) control = 0.0f;
    if (control > 1.0f) control = 1.0f;

    MOTOR_SetSpeed(motor, control);
}

void PIDS_Init(float kp, float ki, float kd) {
    PID_Init(&motorPID[0], kp, ki, kd);

    PID_Init(&motorPID[1], kp, ki, kd);
}
void PID_Init(struct PID *_pid, float kp, float ki, float kd) {
    _pid->kp = kp;
    _pid->ki = ki;
    _pid->kd = kd;
    _pid->integral = 0.0f;
    _pid->lastError = 0.0f;
    _pid->lastDerivative = 0.0f;
}
