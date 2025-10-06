#include <Arduino.h>
#include <LibRobus.h>
#include <DistanceSensor.h>
#include <PID.h>
#include "main.h"
#include "Whistle.h"
//MOTOR de gauche = 0
//MOTOR de droite = 1



///@brief This function is called once before everything by the arduino
///@author Felix
void setup() {
    BoardInit(); // Init the board
    SetupDistanceSensor(); // Setup the distance sensor
    SetupWhistle();
    Serial.begin(115200);//Start Serial monitor display
    //Wait for initialization
    delay(300);
    //Reset both encoders because they are not necessarily reset
    ENCODER_Reset(0);
    ENCODER_Reset(1);


}
///@brief This function simply runs over and over in the runtime. it is called by the Arduino
///@author Felix
void loop() {
    Serial.println("Hello World");
    PrintToString();
    TurnRight();
    delay(1000);

    /*Serial.println("ran");
    Advance(1.0f);
    delay(2000);*/



    /* if(positionY != 9) {
             Logic();
         }*/
}
