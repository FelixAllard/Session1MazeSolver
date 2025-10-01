//
// Created by joshg on 2025-10-01.
//

#include "PID.h"

#include <Arduino.h>
#include <LibRobus.h>

///@var Kp Control period
///@var
typedef struct {
    float Kp, Ki, Kd;
    float integral;
    float prevError;
    float integralLimit;
    float outputLimit;
} PID;
struct PID motorPID[2];  // motorPID[0] for motor 0, motorPID[1] for motor 1




void Advance() {
    //First, create PID and initialize
    PID pid;
    PID_Init(
        pid,
        0.6f,
        0.3f,
        0.02f
    );

    //Now we start the PID Loop
    while (true) {
        delay(SampleMs);
        while (true) {
            delay(SAMPLE_MS);

            // Update motor 0
            PID_ControlMotor(0, setpoint0, dt);

            // Update motor 1
            PID_ControlMotor(1, setpoint1, dt);
        }


    }

}
float PID_Update(struct PID *pid, float setpoint, float measured, float dt) {
    float error = setpoint - measured;

    // Proportional
    float P = pid->kp * error;

    // Integral
    pid->integral += error * dt;
    float I = pid->ki * pid->integral;

    // Derivative
    float derivative = (error - pid->lastError) / dt;
    float D = pid->kd * derivative;

    pid->lastError = error;
    pid->lastDerivative = derivative;

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
void PID_Init(struct PID *pid, float kp, float ki, float kd) {
    pid->kp = kp;
    pid->ki = ki;
    pid->kd = kd;
    pid->integral = 0.0f;
    pid->lastError = 0.0f;
    pid->lastDerivative = 0.0f;
}
