//
// Created by xilef on 10/1/2025.
//

#include "Algorithm.h"
CurrentTile current_tile;

void ResetCurrentTile() {
    current_tile.leftWall=false;
    current_tile.straightWall=false;
    current_tile.rightWall=false;
    current_tile.backWall=false;
    CheckIfTape();
}


void CheckIfTape(){
    if ((positionX == 1) && (positionY %2 != 0)) {
        current_tile.leftWall=true;
        current_tile.straightWall=false;
        current_tile.rightWall=true;
        current_tile.backWall=false;}

    if (positionX == 0) {
        current_tile.leftWall=true;
        current_tile.straightWall=false;
        current_tile.rightWall=false;
        current_tile.backWall=false;}

    if (positionX == 2) {
        current_tile.leftWall=false;
        current_tile.straightWall=false;
        current_tile.rightWall=true;
        current_tile.backWall=false;}

    if (positionY == 0) {
        current_tile.leftWall=false;
        current_tile.straightWall=false;
        current_tile.rightWall=false;
        current_tile.backWall=true;}

    if (positionY == 9) {
        current_tile.leftWall=false;
        current_tile.straightWall=true;
        current_tile.rightWall=false;
        current_tile.backWall=false;}
}

void GetNextMovement() {


}