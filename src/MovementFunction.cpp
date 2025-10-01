#include "MovementFunction.h"
#include <LibRobus.h>

// ---------------- Structure PID simplifiée ----------------
typedef struct {
    float Kp, Ki, Kd;
    float integral;
    float prevError;
} PID;

// ---------------- Configuration PID ----------------
PID pid = {0.8f, 0.1f, 0.05f, 0.0f, 0.0f};

// ---------------- PID avec limites locales ----------------
float computePID(float error, float dt) {
    // Détection de valeurs aberrantes
    if (fabs(error) > 1000) return 0;

    // Limites locales
    const float integralLimit = 50.0f;
    const float outputLimit = 0.4f;

    // Terme proportionnel
    float proportional = pid.Kp * error;

    // Terme intégral avec anti-windup
    pid.integral += error * dt;
    if (pid.integral > integralLimit) pid.integral = integralLimit;
    if (pid.integral < -integralLimit) pid.integral = -integralLimit;
    float integral = pid.Ki * pid.integral;

    // Terme dérivé
    float derivative = pid.Kd * (error - pid.prevError) / dt;
    pid.prevError = error;

    // Calcul de sortie avec saturation
    float output = proportional + integral + derivative;
    if (output > outputLimit) output = outputLimit;
    if (output < -outputLimit) output = -outputLimit;

    return output;
}

// ---------------- Fonction de mouvement améliorée ----------------
void Advance(float targetDistance, float baseSpeed) {
    // Sécurité vitesse
    if (baseSpeed > 0.4f) baseSpeed = 0.4f;
    if (baseSpeed < 0.1f) baseSpeed = 0.1f;

    ResetEncoders();
    delay(50);

    // Calcul du nombre de pulses cible
    // À AJUSTER: remplace 0.1885 par la circonférence réelle de tes roues
    float targetPulses = targetDistance * 3200.0f / 0.1885f;

    float leftPulses = 0, rightPulses = 0;
    unsigned long lastTime = millis();

    // Rampe d'accélération progressive
    float currentSpeed = 0;
    const float acceleration = 0.002f;

    while (currentSpeed < baseSpeed) {
        currentSpeed += acceleration;
        MOTOR_SetSpeed(0, currentSpeed);
        MOTOR_SetSpeed(1, currentSpeed);
        delay(30);
    }

    // Phase de maintien avec PID
    while (leftPulses < targetPulses || rightPulses < targetPulses) {
        unsigned long now = millis();
        float dt = (now - lastTime) / 1000.0f;
        lastTime = now;

        if (dt <= 0 || dt > 0.1f) {
            dt = 0.01f; // Sécurité
        }

        // Lecture des encodeurs
        float leftDelta = ENCODER_Read(0);
        float rightDelta = ENCODER_Read(1);

        // Reset après lecture
        ENCODER_ReadReset(0);
        ENCODER_ReadReset(1);

        // Filtrage des valeurs aberrantes
        if (fabs(leftDelta) > 1000) leftDelta = 0;
        if (fabs(rightDelta) > 1000) rightDelta = 0;

        leftPulses += leftDelta;
        rightPulses += rightDelta;

        // Calcul de l'erreur
        float error = leftPulses - rightPulses;

        // Zone morte
        if (fabs(error) < 5) error = 0;

        // Calcul de correction PID
        float correction = computePID(error, dt);

        // Application des vitesses
        float leftSpeed = currentSpeed - correction;
        float rightSpeed = currentSpeed + correction;

        // Saturation
        if (leftSpeed > 0.6f) leftSpeed = 0.6f;
        if (leftSpeed < -0.6f) leftSpeed = -0.6f;
        if (rightSpeed > 0.6f) rightSpeed = 0.6f;
        if (rightSpeed < -0.6f) rightSpeed = -0.6f;

        MOTOR_SetSpeed(0, leftSpeed);
        MOTOR_SetSpeed(1, rightSpeed);

        // Condition de sortie
        if (leftPulses >= targetPulses && rightPulses >= targetPulses) {
            break;
        }

        delay(10);
    }

    // Arrêt progressif
    const float deceleration = 0.005f;
    while (currentSpeed > 0) {
        currentSpeed -= deceleration;
        if (currentSpeed < 0) currentSpeed = 0;
        MOTOR_SetSpeed(0, currentSpeed);
        MOTOR_SetSpeed(1, currentSpeed);
        delay(20);
    }

    MOTOR_SetSpeed(0, 0);
    MOTOR_SetSpeed(1, 0);

    // Reset de l'intégrateur
    pid.integral = 0;
}

// ---------------- Fonctions helper ----------------
void ResetEncoders() {
    ENCODER_Reset(0);
    ENCODER_Reset(1);
}

void StartAdvancing(float speed) {
    MOTOR_SetSpeed(0, speed);
    MOTOR_SetSpeed(1, speed);
}

float GetRotationWheel(float pulse) {
    return pulse / 3200.0f; // À ajuster selon ton robot
}

// ---------------- Fonctions de virage simplifiées ----------------
void TurnLeft(float angleDegrees) {
    ResetEncoders();

    // À AJUSTER: calibration pour ton robot
    float targetPulses = (angleDegrees / 90.0f) * 800.0f;

    float leftPulses = 0, rightPulses = 0;

    while (leftPulses > -targetPulses || rightPulses < targetPulses) {
        leftPulses += ENCODER_ReadReset(0);
        rightPulses += ENCODER_ReadReset(1);

        MOTOR_SetSpeed(0, -0.3f);
        MOTOR_SetSpeed(1, 0.3f);

        delay(10);
    }

    MOTOR_SetSpeed(0, 0);
    MOTOR_SetSpeed(1, 0);
}

void TurnRight(float angleDegrees) {
    ResetEncoders();

    // À AJUSTER: calibration pour ton robot
    float targetPulses = (angleDegrees / 90.0f) * 800.0f;

    float leftPulses = 0, rightPulses = 0;

    while (leftPulses < targetPulses || rightPulses > -targetPulses) {
        leftPulses += ENCODER_ReadReset(0);
        rightPulses += ENCODER_ReadReset(1);

        MOTOR_SetSpeed(0, 0.3f);
        MOTOR_SetSpeed(1, -0.3f);

        delay(10);
    }

    MOTOR_SetSpeed(0, 0);
    MOTOR_SetSpeed(1, 0);
}