//
// Created by Felix on 2025-10-01.
//

#ifndef SESSION1MAZESOLVER_PID_H
#define SESSION1MAZESOLVER_PID_H


/// @brief Number of pulses per rotation (encoder resolution)
const float PPR = 3200.0f;

/// @brief Sample period (ms) for control loop
const float SampleMs = 63;

/// @brief Sample period (s) for control loop
const float SampleS = SampleMs / 1000.0f;

/// @brief Motor top speed (rotations per second, RPS)
const float maxRPS = 1.0f;

/// @brief Last encoder readings (global for both motors)
extern long lastCountEncoder[2];

/// @brief Accumulated encoder counts (for synchronization correction)
extern long totalCountEncoder[2];

/// @brief Discrete PID controller state and parameters
struct PID {
    float kp;             ///< Proportional gain
    float ki;             ///< Integral gain
    float kd;             ///< Derivative gain
    float integral;       ///< Integral accumulator (with clamping)
    float lastError;      ///< Previous loop error
    float lastDerivative; ///< Previous derivative
};

/// @brief Two PID controllers (one for each motor)
extern PID motorPID[2];

/// @brief Functions
void Advance(float targetSpeed);                                   ///< Run main loop

void TurnLeft();
void TurnRight();
void PID_Init(PID* _pid, float kp, float ki, float kd);            ///< Init one PID
void PIDS_Init(float kp, float ki, float kd);                      ///< Init both PIDs
float PID_Update(PID* Pid, float setpoint, float measured, float dt); ///< Update one PID
void PID_ControlMotors(float targetSpeed);
#endif //SESSION1MAZESOLVER_PID_H