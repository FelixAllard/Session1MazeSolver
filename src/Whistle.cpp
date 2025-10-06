//
// Created by xilef on 10/1/2025.
//

#include "Whistle.h"
#include <stdio.h>
#include <Arduino.h>
#include <LibRobus.h>
int lastRead;
void SetupWhistle() {
    pinMode(pinBruitAmbiant, INPUT) ;
    pinMode(pinIntensite_5HZ, INPUT) ;
}

float GetFrequence() {
}

bool Micro() {
}

bool Sifflet() {
}

void PrintToString() {
    //Serial.println(analogRead(pinBruitAmbiant)) ;
    int result = analogRead(pinIntensite_5HZ);
    Serial.println(result) ;
    Serial.println(lastRead-result);
    lastRead = result;
    delay(300);

}
