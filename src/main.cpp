#include <Arduino.h>
#include <LibRobus.h>
#include <DistanceSensor.h>
#include <PID.h>
#include "main.h"
#include "Whistle.h"
//MOTOR de gauche = 0
//MOTOR de droite = 1

void setup() {
    BoardInit(); // Init the board
    SetupDistanceSensor(); // Setup the distance sensor
    SetupWhistle();
    Serial.begin(115200);//Start Serial monitor display
    delay(300);
    ENCODER_Reset(0);
    ENCODER_Reset(1);



    /*CheckIfWallInFront(0.25f);
    MOTOR_SetSpeed(0, 0.1f);
    MOTOR_SetSpeed(1, 0.1f);
    delay(5000);
    MOTOR_SetSpeed(0, 0);
    MOTOR_SetSpeed(1, 0);
    //Advance(0.3f, 15);*/



}

void loop() {
    Serial.println("Hello World");
    PrintToString();
}
