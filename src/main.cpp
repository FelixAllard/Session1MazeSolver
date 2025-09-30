#include <Arduino.h>
#include <LibRobus.h>

#include "MovementFunction.h"
void setup() {
    BoardInit();
    MOTOR_SetSpeed(0, 0.1f);
    delay(5000);
    MOTOR_SetSpeed(0, 0);
    //Advance(0.2f, 50);


}

void loop() {

}
