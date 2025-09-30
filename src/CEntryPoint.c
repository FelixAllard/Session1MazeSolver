//
// Created by xilef on 9/30/2025.
//

#include "CEntryPoint.h"
#include <LibRobus.h>

///@brief First thing to run in C
void SetupC() {
    BoardInit();
    // GIVE A LITTLE TIME FOR BOARD TO INIT
    delay(1000);
    MOTOR_SetSpeed(1, 0.1);
    MOTOR_SetSpeed(0, 0.1);
    delay(5000);
    MOTOR_SetSpeed(1, 0);
    MOTOR_SetSpeed(0, 0);

}
///@brief Loop in C, all logic should pass by here
void LoopC() {

}
