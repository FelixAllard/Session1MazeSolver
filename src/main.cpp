#include <Arduino.h>
#include <LibRobus.h>

#include "MovementFunction.h"
//MOTOR de gauche = 0
//MOTOR de droite = 1
void setup() {
    BoardInit();
    delay(1000);
    Advance(0.3f, 15);


}

void loop() {

}
