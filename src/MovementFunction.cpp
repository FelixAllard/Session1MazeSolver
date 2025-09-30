//
// Created by xilef on 9/30/2025.
//

#include "MovementFunction.h"
#include <LibRobus.h>
//
// Created by xilef on 9/30/2025.
//

#include "MovementFunction.h"
#include <LibRobus.h>

// ---------------- PID Structure ----------------
struct PID {
    float Kp;
    float Ki;
    float Kd;
    float prevError;
    float integral;
};

PID pid = {0.1f, 0.0f, 0.01f, 0.0f, 0.0f}; // <-- start with Kp only, tune later

float computePID(PID &pid, float error, float dt) {
    pid.integral += error * dt;
    float derivative = (error - pid.prevError) / dt;
    float output = pid.Kp * error + pid.Ki * pid.integral + pid.Kd * derivative;
    pid.prevError = error;
    return output;
}

// ---------------- Movement Functions ----------------

void Advance(float baseSpeed = 0.5f, float wheelRotation = 5) {
    float leftWheelPulse = 0, rightWheelPulse = 0;

    ResetEncoders();
    delay(100);

    unsigned long lastTime = millis();
    StartAdvancing(baseSpeed);

    while (true) {
        delay(50);  // control loop period (50 ms)

        unsigned long now = millis();
        float dt = (now - lastTime) / 1000.0f; // seconds
        lastTime = now;

        // accumulate encoder pulses
        leftWheelPulse  += ENCODER_ReadReset(0);
        rightWheelPulse += ENCODER_ReadReset(1);

        // error = difference between wheels
        float error = leftWheelPulse - rightWheelPulse;

        // PID correction
        float correction = computePID(pid, error, dt);

        // Apply correction
        float leftMotorSpeed  = baseSpeed - correction;
        float rightMotorSpeed = baseSpeed + correction;

        // clamp between -1 and 1
        if (leftMotorSpeed > 1) leftMotorSpeed = 1;
        if (leftMotorSpeed < -1) leftMotorSpeed = -1;
        if (rightMotorSpeed > 1) rightMotorSpeed = 1;
        if (rightMotorSpeed < -1) rightMotorSpeed = -1;

        MOTOR_SetSpeed(0, leftMotorSpeed);
        MOTOR_SetSpeed(1, rightMotorSpeed);

        // stop condition
        if (GetRotationWheel(leftWheelPulse) >= wheelRotation &&
            GetRotationWheel(rightWheelPulse) >= wheelRotation) {
            break;
        }
    }

    MOTOR_SetSpeed(0, 0);
    MOTOR_SetSpeed(1, 0);
}

void TurnLeft() {
    // TODO: implement turn with encoders
}

void TurnRight() {
    // TODO: implement turn with encoders
}

// ---------------- Simplicity Functions ----------------

void ResetEncoders() {
    ENCODER_Reset(0);
    ENCODER_Reset(1);
}

void StartAdvancing(float speed) {
    MOTOR_SetSpeed(0, speed);
    MOTOR_SetSpeed(1, speed);
}

float GetRotationWheel(float pulse) {
    return pulse / 3200.0f; // pulses -> rotations
}

/*void Advance(float speed = 0.5f, float wheelRotation = 5) {
    float leftWheelPulse = 0, rightWheelPulse = 0;
    float leftMotorSpeed = speed, rightMotorSpeed = speed;
    ResetEncoders();
    delay(100);
    StartAdvancing(speed);
    {
        delay(100);
        leftWheelPulse += ENCODER_ReadReset(0);
        rightWheelPulse += ENCODER_ReadReset(1);
        if (leftWheelPulse > rightWheelPulse) {
            leftMotorSpeed -= 0.01;
            rightMotorSpeed += 0.01;
            MOTOR_SetSpeed(0, leftMotorSpeed);
            MOTOR_SetSpeed(1, rightMotorSpeed);
        }
        else if (leftWheelPulse < rightWheelPulse) {
            leftMotorSpeed -= 0.01;
            rightMotorSpeed += 0.01;
            MOTOR_SetSpeed(0, leftMotorSpeed);
            MOTOR_SetSpeed(1, rightMotorSpeed);
        }

    }while (GetRotationWheel(leftWheelPulse)>= wheelRotation && GetRotationWheel(rightWheelPulse) >= wheelRotation);
}





void TurnLeft() {

}
void TurnRight() {

}

//--------------------------------------------------Simplicity Functions -----------------------------------------------


void ResetEncoders() {
    ENCODER_Reset(0);
    ENCODER_Reset(1);
}
void StartAdvancing(float speed) {
    MOTOR_SetSpeed(0, speed);
    MOTOR_SetSpeed(1, speed);
}

float GetRotationWheel(float pulse) {
    float wheelRotation = pulse/3200;
    return wheelRotation;
}*/