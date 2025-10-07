//
// Created by xilef on 10/1/2025.
//

#include "Algorithm.h"

#include <Arduino.h>

#include "DistanceSensor.h"
#include "PID.h"
#include "TurnPID.h"

int positionXAlgo =1;
int positionYAlgo = 0;
int previousDirection = 0;
int facingDirection = 1;
int nextDirection = 0;
int  defaultTurn = 0;


CurrentTile current_tile;

void ResetCurrentTile() {
    current_tile.leftWall=false;
    current_tile.straightWall=false;
    current_tile.rightWall=false;
    current_tile.backWall=false;
    CheckIfTape();
}


void CheckIfTape(){
    if (positionYAlgo %2 != 0) {
        current_tile.leftWall=true;
        current_tile.rightWall=true;
    }

    if (positionXAlgo == 0) {
        current_tile.leftWall=true;
    }
    if (positionXAlgo == 2) {
        current_tile.rightWall=true;
    }

    if (positionYAlgo == 0) {
        current_tile.backWall=true;
    }
}


///@brief Check if there is a wall in front and note it in the CurrentTile
bool TestFrontWall() {
    bool wallInFront = GetBothSensor();
    if (wallInFront) {
        if (facingDirection == 0) {
            current_tile.leftWall = true ;
        }
        if (facingDirection == 1) {
            current_tile.straightWall = true ;
        }
        if (facingDirection == 2) {
            current_tile.rightWall = true ;
        }
        if (facingDirection == 3) {
            current_tile.backWall = true ;
        }
        return true ;
    }
    return false ;
}

/**
 * @brief Choisit le prochain mouvement à effectuer.
 * Retourne :
 *   0 = avancer
 *   1 = tourner à gauche
 *   2 = tourner à droite
 *   3 = demi-tour
 */

int GetNextMovement() {
    // priorité : avancer > gauche > droite > demi-tour
    if (!current_tile.straightWall)
    {
        if (facingDirection == 2) {
            defaultTurn = 2;
            return 0 ;
        }
        if (facingDirection == 0) {
            defaultTurn = 0;
            return 2 ;
        }
    }
    // On verifie le mur devant soi
    if (!current_tile.straightWall) {
        if (facingDirection == 1) {
            return 1 ;
        }
    }

    // On verifie le mur a gauche
    if (!current_tile.leftWall) {
        if (facingDirection == 0) {
            return 1 ;
        }
    }

    // On verifie le mur a droite
    if (!current_tile.rightWall) {
        if (facingDirection == 2) {
            return 1 ;
        }
    }

    // On verifie le mur derriere soi
    if (!current_tile.backWall) {
        if (facingDirection == 3) {
            return 1 ;
        }
    }

    if (current_tile.straightWall && current_tile.leftWall && current_tile.rightWall) {
        if (facingDirection == 1) {
            return 0 ;
        }
        if (facingDirection == 0) {
            return 0 ;
        }
        if (facingDirection == 2) {
            return 2 ;
        }
    }

    if (current_tile.straightWall && current_tile.leftWall) {
        if (facingDirection == 1) {
            return 2 ;
        }
        if (facingDirection == 0) {
            return 2 ;
        }
    }

    if (current_tile.straightWall && current_tile.rightWall) {
        if (facingDirection == 1) {
            return 0 ;
        }
        if (facingDirection == 2) {
            return 0 ;
        }
    }
    if (current_tile.straightWall) {
        if (facingDirection == 1) {
            return defaultTurn ; //ajoute par theo
        }
    }
    if (current_tile.leftWall) {
        if (facingDirection == 0) {
            return 2 ;
        }
    }
    if (current_tile.rightWall) {
        if (facingDirection == 2) {
            return 0 ;
        }
    }
    if (current_tile.straightWall) {
        if (facingDirection == 1) {
            return 1 ;
        }
    }
    //TODO if in middle so x1, turn to face the the end AND MAKE SURE HE DOESN't go back. This is harder than just turn, you have to make sure

    return 1;

}
///@authors Daniela, Felix
///@brief Logic is responsible for bringing the algorithme together. it is called in a loop iteration in the Loop function of main.cpp
void Logic() {
    //We first Reset the current tile
    ResetCurrentTile();
    //this loop is so that it repeats again and again until it advance
    while (true) {
        bool test =TestFrontWall();

        int nextMovement = GetNextMovement();

        Serial.print(positionXAlgo);
        Serial.println(positionYAlgo);
        Serial.print(test);
        Serial.println(nextMovement);
        //Turn left

        switch (nextMovement) {
            case 0:
                TurnLeft();
                facingDirection -=1;
                if (facingDirection == -1) {
                    facingDirection = 3;
                }
                break;

            case 2:

                TurnRight();
                facingDirection +=1;
                if (facingDirection == 4) {
                    facingDirection = 0;
                }
                break;
            case 1:
                AdvanceDistance(0.384f, 1.0f);
                if (facingDirection == 0) {
                    positionXAlgo--;
                }
                if (facingDirection == 1) {
                    positionYAlgo++;
                }
                if (facingDirection == 2) {
                    positionXAlgo++;
                }
                if (facingDirection == 3) {
                    positionYAlgo--;
                }

                return;

                break;
            default:

                Serial.println("We reached nothing. WTF");
                return;
        }
    }
}
