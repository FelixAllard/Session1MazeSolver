//
// Created by xilef on 10/8/2025.
//

#include "MotorBias.h"


//1 = white board , 2 = transparent board
const int ROBOT_NUMBER = 1;

float motorBias_Base[2] = {1.0f, 1.0f};  // default

void SetMotorBias() {
    if (ROBOT_NUMBER ==1) {
        motorBias_Base[1] = 1.016f;
    }
    else if (ROBOT_NUMBER == 2) {
        motorBias_Base[1] = 1.0315f;
    }
}

//White board
#if BOARD_ID == 1
float newBias[2] = {1.0f, 1.016};
SetMotorBias(newBias);

//Transparent board
#elif BOARD_ID == 2
float newBias[2] = {1.00f, 1.0315f};
SetMotorBias(newBias);
#endif
