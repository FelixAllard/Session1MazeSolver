//
// Created by Felix on 2025-10-01.
//

#ifndef SESSION1MAZESOLVER_PID_H
#define SESSION1MAZESOLVER_PID_H

///@brief Number of pulse per rotation
const float PulsePerRotation = 3200;
///@brief Number ms for each sample
const float SampleMs = 300;
///@brief Number of Seconds per each sample
const float SampleS = SampleMs / 1000.0;

///@brief Number of pulse / rotation
const float PPR = 3200.0f;

///@brief Motor top speed
const float maxRPS = 1.0f;

extern long lastCountEncoder[2];

/// @brief Discrete PID controller state and parameters
/// @author Felix Allard
struct PID {
    float kp;             ///< Proportional gain: scales current error response
    float ki;             ///< Integral gain: scales accumulated error over time
    float kd;             ///< Derivative gain: scales rate of error change (damping)
    float integral;       ///< Accumulated error sum (I-term memory, with clamping)
    float lastError;      ///< Previous loop error (for derivative calculation)
    float lastDerivative; ///< Previous derivative value (for smoothing/filtering)
};

extern PID pid;

/// @brief Functions
void Advance(float targetSpeed);
void PID_Init(PID* _pid, float kp, float ki, float kd);
void PIDS_Init(float kp, float ki, float kd);
float PID_Update(PID* Pid, float setpoint, float measured, float dt);
void PID_ControlMotors(float targetSpeed);

#endif //SESSION1MAZESOLVER_PID_H