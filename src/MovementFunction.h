//
// Created by xilef on 9/30/2025.
//

#ifndef SESSION1MAZESOLVER_MOVEMENTFUNCTION_H
#define SESSION1MAZESOLVER_MOVEMENTFUNCTION_H

#include <Arduino.h>   // for millis(), delay()
#include <LibRobus.h>  // motor + encoder functions

// ---------------- PID Structure ----------------
struct PID {
    float Kp;
    float Ki;
    float Kd;
    float prevError;
    float integral;
};

// ---------------- Function Prototypes ----------------

// Core movement
void Advance(float baseSpeed = 0.5f, float wheelRotation = 5);
void TurnLeft();
void TurnRight();

// Helpers
void ResetEncoders();
void StartAdvancing(float speed);
float GetRotationWheel(float pulse);

// PID helper
float computePID(PID &pid, float error, float dt);
#endif //SESSION1MAZESOLVER_MOVEMENTFUNCTION_H