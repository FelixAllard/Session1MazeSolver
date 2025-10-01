//
// Created by joshg on 2025-10-01.
#include <LibRobus.h>
#include <Arduino.h>


const int Prox_G = 53;
const int Prox_D = 49;
bool WallD = FALSE;
bool WallG = FALSE;

void setup()
{
    pin_mode(Prox_G, INPUT);
    pin_mode(Prox_D, INPUT);
}

void loop()
{
   findMur();
}

bool findMur(Prox_G,Prox_D)
{
    WallD = digitalRead(Prox_D);
    WallG = digitalRead(Prox_G);

if (WallD  == HIGH && WallG == HIGH)
    {
    return 1;

else if (WallD == HIGH && WallG == LOW)
    return 2;
else if (WallD == LOW && WallG == HIGH)
    return 3;
else
    return 0;
    }

}