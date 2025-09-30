#include <Arduino.h>
#include <LibRobus.h>

#include "MovementFunction.h"
//MOTOR de gauche = 0
//MOTOR de droite = 1
void setup() {
    BoardInit();
    Advance(0.2f, 50);


}

void loop() {

}
