//
// Created by xilef on 10/1/2025.
//

#include "DistanceSensor.h"
#include <Arduino.h>
//Check header file for description of each functions

void SetupDistanceSensor() {
    pinMode(redPin, INPUT_PULLUP);
    pinMode(greenPin, INPUT_PULLUP);

}
bool GetRedSensor() {
    bool red = !digitalRead(redPin);

    return red;
}
bool GetGreenSensor() {
    bool green = !digitalRead(greenPin);

    return green;
}
bool GetBothSensor() {

    bool red = !digitalRead(redPin);
    bool green = !digitalRead(greenPin);

    if (red && green) {
        return true;
    }
    else {
        return false;
    }

}