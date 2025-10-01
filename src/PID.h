//
// Created by joshg on 2025-10-01.
//

#ifndef SESSION1MAZESOLVER_PID_H
#define SESSION1MAZESOLVER_PID_H

///@brief Number of pulse per rotation
const float PulsePerRotation = 3200;
///@brief Number ms for each sample
const float SampleMs = 30.0;
///@brief Number of Seconds per each sample
const float SampleS = SampleMs / 1000.0;

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

void Advance();
void PID_Init(struct PID *pid, float kp, float ki, float kd);
void PIDS_Init(float kp, float ki, float kd);
void PID_ControlMotor(int motor, float setpoint, float dt);
float computePID(PID &pid, float error, float dt);
#endif //SESSION1MAZESOLVER_PID_H