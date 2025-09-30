#include <Arduino.h>
#include <LibRobus.h>

void setup() {
    BoardInit();
    // GIVE A LITTLE TIME FOR BOARD TO INIT
    delay(1000);
    MOTOR_SetSpeed(1, 0.1);
    MOTOR_SetSpeed(0, 0.1);
    delay(5000);
    MOTOR_SetSpeed(1, 0);
    MOTOR_SetSpeed(0, 0);

}

void loop() {

}