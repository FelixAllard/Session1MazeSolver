//
// Created by xilef on 10/1/2025.
//

#include "Whistle.h"
#include <stdio.h>
#include <Arduino.h>
#include <LibRobus.h>

int lastRead;
int result =0;
int count=0;
int count5Hz =0;
unsigned long lastUpdate = 0;
bool start =false;


void SetupWhistle() {
    pinMode(pinBruitAmbiant, INPUT) ;
    pinMode(pinIntensite_5HZ, INPUT) ;
}



bool Avertissement() {

    if (millis() - lastUpdate < 1000) {
        return false;
    }

    lastUpdate = millis();

    result = analogRead(pinBruitAmbiant) - analogRead(pinIntensite_5HZ);

    if (result < -28) {
        count++;
    } else {
        return false;
    }

        if (count % 6 == 0) {
            for (int i = 0; i < 10; i++) {
                Serial.println(i+1);

                delay(1000);
            }

            return true;
    }

    if (count % 3 == 0) {
        Serial.println(" Avertissement détecté 1kHz");
        return true;
    }

    return false;
}

bool SiffletStart() {
    result = analogRead(pinBruitAmbiant) - analogRead(pinIntensite_5HZ);

    if (result>70)
    {
        start = true;
      return true;
    }else{
        return false;

    }
}

void PrintToString() {
    Serial.println(analogRead(pinBruitAmbiant)) ;

    Serial.println(analogRead(pinIntensite_5HZ)) ;
    Serial.println(analogRead(pinBruitAmbiant)) ;


   Serial.println(result);
    Serial.println(SiffletStart());


    if (Avertissement()) {
        Serial.println("Un Avertissement 1 kHz détecté");
    }
    delay(300);
}
