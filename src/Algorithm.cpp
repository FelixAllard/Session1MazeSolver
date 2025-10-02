//
// Created by xilef on 10/1/2025.
//

#include "Algorithm.h"
#include "DistanceSensor.h"

CurrentTile current_tile;

void CheckIfTape(){
    if ((positionY %2 != 0) && (positionX == 1)) {

        current_tile.leftWall=true;
        current_tile.straightWall=false;
        current_tile.rightWall=true;
        current_tile.backWall=false;
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