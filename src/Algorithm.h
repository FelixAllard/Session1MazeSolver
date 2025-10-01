//
// Created by xilef on 10/1/2025.
//

#ifndef SESSION1MAZESOLVER_ALGORITHM_H
#define SESSION1MAZESOLVER_ALGORITHM_H

///Current position so we can find the tape. If x is 0, left wall is a tape. If x is 2, right wall is a tape
int positionX = 1;
///Current position on y. If y is 0, then back wall is a real wall. If y is odd, then left and right wall are tape
int positionY = 0;

///@brief 0 = Left, 1 = Straight, 2 = Right, 3 = Back
int previousDirection;


///@brief 0 = Left, 1 = Straight, 2 = Right, 3 = Back
int facingDirection;


///@brief 0 = Left, 1 = Straight, 2 = Right, 3 = Back
int nextDirection;

///@authors Felix, Daniela
///@date 10/1/2025
struct CurrentTile {
    bool leftWall = false;
    bool straightWall = false;
    bool rightWall = false;
    bool backWall = false;
};


void InitAlgorithm();

///Reset the current tile and should also modivy the struct depending on what the tape is like. ( Call CheckIfTape() function)
void ResetCurrentTile();

///Check with current cardinal coordinate to know if there is a tape around the robot. Called before checking any walls.
void CheckIfTape();

///@brief Returns the next movement for the robot
///< 0 = Turn left, 1 = Go straight, 2 = Turn Right
int GetNextMovement();

///@brief Check if there is a wall in front and note it in the CurrentTile
bool TestFrontWall();



#endif //SESSION1MAZESOLVER_ALGORITHM_H