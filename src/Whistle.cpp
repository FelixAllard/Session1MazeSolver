//
// Created by xilef on 10/1/2025.
//

#include "Whistle.h"
#include <stdio.h>
#include <Arduino.h>
#include <LibRobus.h>

void SetupWhistle() {
    pinMode(pinBruit, INPUT) ;
    pinMode(pinIntensite, INPUT) ;
}

float GetFrequence() {
}

bool Micro() {
}

bool Sifflet() {
}

void PrintToString() {
    Serial.println(digitalRead(pinBruit)) ;
    Serial.println(digitalRead(pinIntensite)) ;

}
