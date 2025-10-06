//
// Created by xilef on 10/1/2025.
//

#include "Algorithm.h"

#include <Arduino.h>

#include "DistanceSensor.h"
#include "PID.h"
#include "TurnPID.h"

void DetectHz() {

    loop();
}



CurrentTile current_tile;

void ResetCurrentTile() {
    current_tile.leftWall=false;
    current_tile.straightWall=false;
    current_tile.rightWall=false;
    current_tile.backWall=false;
    CheckIfTape();
}


void CheckIfTape(){
    if (positionY %2 != 0) {
        current_tile.leftWall=true;
        current_tile.rightWall=true;
    }

    if (positionX == 0) {
        current_tile.leftWall=true;
    }
    //TODO make sure condition don't overide each other
    if (positionX == 2) {
        current_tile.rightWall=true;
    }

    if (positionY == 0) {
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
            return 0 ;
        }
    }

    return 1;
    //TODO some condit
    //If looking toward right and wall in front, turn to left

    //If looking toward left and wall in front, turn to right

    //If looking straight and wall in front, but wall neither left nor right, turn left ( or right, your choice )
    // wall in front, left and right, turn to face backward (fastest way)

}
///@authors Daniela, Felix
///@brief Logic is responsible for bringing the algorithme together. it is called in a loop iteration in the Loop function of main.cpp
void Logic() {
    //We first Reset the current tile
    ResetCurrentTile();
    //this loop is so that it repeats again and again until it advance
    while (true) {
        bool WallInFront = TestFrontWall();
        int nextMovement = GetNextMovement();
        //Turn left
        if (nextMovement == 0) {
            TurnLeft();
            facingDirection -=1;
            if (facingDirection == -1) {
                facingDirection = 3;
            }
        }
        //Turn right
        if (nextMovement == 2) {
            TurnRight();
            facingDirection +=1;
            if (facingDirection == 4) {
                facingDirection = 0;
            }
        }
        //Avance, the return ends the function.
        if (nextMovement == 1) {
            AdvanceDistance(0.45f, 1.0f);
            if (facingDirection == 0) {
                positionX--;
            }
            if (facingDirection == 1) {
                positionY++;
            }
            if (facingDirection == 2) {
                positionX++;
            }
            if (facingDirection == 3) {
                positionY--;
            }
            return;
        }
    }
}



