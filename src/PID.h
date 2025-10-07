//
// Created by Felix on 2025-10-01.
//

#ifndef SESSION1MAZESOLVER_PID_H
#define SESSION1MAZESOLVER_PID_H



/// @brief Number of pulses per rotation (encoder resolution)
/// @author Felix
const float PPR = 3200.0f;

/// @brief Sample period (ms) for control loop
/// @author Felix
const float SampleMs = 150;

/// @brief Sample period (s) for control loop
/// @author Felix
const float SampleS = SampleMs / 1000.0f;

/// @brief Motor top speed (rotations per second, RPS)
/// @author Felix
const float maxRPS = 15.0f;

/// @brief Last encoder readings (global for both motors)
/// @author Felix
extern long lastCountEncoder[2];

/// @brief Accumulated encoder counts (for synchronization correction)
/// @author Felix
extern long totalCountEncoder[2];

///@brief Discrete PID controller state and parameters
///@author Felix
struct PID {
    float kp;             ///< Proportional gain
    float ki;             ///< Integral gain
    float kd;             ///< Derivative gain
    float integral;       ///< Integral accumulator (with clamping)
    float lastError;      ///< Previous loop error
    float lastDerivative; ///< Previous derivative
};

/// @brief Two PID controllers (one for each motor)
/// @author Felix
extern PID motorPID[2];

/// @brief Function that advances exactly 0.5m
/// @author Felix
void Advance(float targetSpeed = 1.0f);                                   ///< Run main loop
void AdvanceDistance(float distanceMeters, float fractionSpeed = 1.0f);

/*///@brief function that turns 90 degrees left
///@exception NotImplemented
///@author Felix
void TurnLeft();
///@brief Function that turns 90 degrees right
///@exception NotImplemented
///@author Felix
void TurnRight();*/
void PID_Init(PID* _pid, float kp, float ki, float kd);            ///< Init one PID
void PIDS_Init(float kp, float ki, float kd);                      ///< Init both PIDs
float PID_Update(PID* Pid, float setpoint, float measured, float dt); ///< Update one PID
void PID_ControlMotors(float targetSpeed);
#endif //SESSION1MAZESOLVER_PID_H