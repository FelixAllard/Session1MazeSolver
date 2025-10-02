//
// Created by joshg on 2025-10-01.
//

#include "PID.h"

#include <Arduino.h>
#include <LibRobus.h>

///@var Kp Control period
///@var

struct PID motorPID[2];  // motorPID[0] for motor 0, motorPID[1] for motor 1




void Advance(float targetSpeed) {
    const int SampleMs = 20;                 // update every 20 ms
    const float dt = SampleMs / 1000.0f;     // convert ms → seconds

    // Initialize both motor PIDs
    PIDS_Init(0.6f, 0.3f, 0.02f);

    // Main PID loop
    while (true) {
        delay(SampleMs);

        // Update both motors
        PID_ControlMotor(0, targetSpeed, dt);
        PID_ControlMotor(1, targetSpeed, dt);
    }
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
    // Read encoder for this motor
    int counts = ENCODER_ReadReset(motor);  // motor = 0 or 1
    float rotations = counts / PPR;
    float measuredRPS = rotations / dt;
    float measured = measuredRPS / maxRPS;
    if (measured > 1.0f) measured = 1.0f;

    // Compute PID output
    float control = PID_Update(&motorPID[motor], setpoint, measured, dt);

    // Clamp to 0..1
    if (control < 0.0f) control = 0.0f;
    if (control > 1.0f) control = 1.0f;

    // Send to motor
    MOTOR_SetSpeed(motor, control);
}

void PIDS_Init(float kp, float ki, float kd) {
    PID_Init(&motorPID[0], kp, ki, 0.02f);
    PID_Init(&motorPID[1], kp, ki, 0.02f);
}
void PID_Init(struct PID *_pid, float kp, float ki, float kd) {
    _pid->kp = kp;
    _pid->ki = ki;
    _pid->kd = kd;
    _pid->integral = 0.0f;
    _pid->lastError = 0.0f;
    _pid->lastDerivative = 0.0f;
}
