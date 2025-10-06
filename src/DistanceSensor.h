//
// Created by xilef on 10/1/2025.
//

#ifndef SESSION1MAZESOLVER_DISTANCESENSOR_H
#define SESSION1MAZESOLVER_DISTANCESENSOR_H
///@brief Where the green pin sensor is connected
///@author Felix
const int greenPin = 49;
///@brief Where the red pin sensor is connected
///@author Felix
const int redPin = 53;
///@brief A setup function that needs to be executed once before the program uses any other function of the DistanceSensor
///@author Felix
void SetupDistanceSensor();
///@brief Check if the red sensor finds something before itself
///@author Felix
bool GetRedSensor();
///@brief Check if the green sensor finds something before itself
///@author Felix
bool GetGreenSensor();
///@brief Check if both sensor find a wall.
///@note If both have a wall = true, else false
///@author Felix
bool GetBothSensor();

#endif //SESSION1MAZESOLVER_DISTANCESENSOR_H