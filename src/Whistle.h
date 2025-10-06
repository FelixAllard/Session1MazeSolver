//
// Created by xilef on 10/1/2025.
//

#ifndef SESSION1MAZESOLVER_WHISTLE_H

#define SESSION1MAZESOLVER_WHISTLE_H
#include <Arduino.h>
const int pinIntensite_5HZ = A1 ;
const int pinBruitAmbiant = A0 ;

void SetupWhistle() ;

bool Avertissement();

bool SiffletStart() ;

void PrintToString();
#endif //SESSION1MAZESOLVER_WHISTLE_H
