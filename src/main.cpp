#include <Arduino.h>
#include <LibRobus.h>
#include <DistanceSensor.h>
#include "main.h"
//MOTOR de gauche = 0
//MOTOR de droite = 1

void setup() {
    BoardInit();
    SetupDistanceSensor();
    Serial.begin(115200);


    delay(1000);

    /*CheckIfWallInFront(0.25f);
    MOTOR_SetSpeed(0, 0.1f);
    MOTOR_SetSpeed(1, 0.1f);
    delay(5000);
    MOTOR_SetSpeed(0, 0);
    MOTOR_SetSpeed(1, 0);
    //Advance(0.3f, 15);*/



}

void loop() {
    Serial.print("Sonar 0 = " );
    Serial.print(GetBothSensor());

    Serial.print("\n");
    delay(100);
}
