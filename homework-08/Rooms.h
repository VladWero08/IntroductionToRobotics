#pragma once
#ifndef ROOMS_H
#define ROOMS_H

const byte directions = 4;
const byte roomsSize = 4;
const byte matrixSize = 8;
// array which contains the initial configuration
// for each room in the game
const bool rooms[roomsSize][matrixSize][matrixSize] = {
  {
    {1, 1, 1, 0, 0, 1, 1, 1},
    {1, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 1, 0, 1, 0, 0, 1},
    {0, 0, 0, 0, 1, 1, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0},
    {1, 0, 1, 1, 1, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 1},
    {1, 1, 1, 0, 0, 1, 1, 1}
  },
  {
    {1, 1, 1, 0, 0, 1, 1, 1},
    {1, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 1, 0, 0, 1, 0, 1},
    {0, 0, 1, 1, 0, 0, 0, 0},
    {0, 0, 0, 1, 0, 0, 0, 0},
    {1, 0, 1, 1, 0, 1, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 1},
    {1, 1, 1, 0, 0, 1, 1, 1}
  },
  {
    {1, 1, 1, 0, 0, 1, 1, 1},
    {1, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 1, 1, 0, 0, 0, 1},
    {0, 0, 0, 1, 1, 0, 0, 0},
    {0, 0, 0, 0, 1, 0, 0, 0},
    {1, 0, 1, 1, 0, 1, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 1},
    {1, 1, 1, 0, 0, 1, 1, 1}
  },
  {
    {1, 1, 1, 0, 0, 1, 1, 1},
    {1, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 1, 1, 1, 1, 0, 1},
    {0, 0, 0, 1, 0, 0, 0, 0},
    {0, 0, 0, 1, 0, 0, 0, 0},
    {1, 0, 1, 1, 0, 1, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 1},
    {1, 1, 1, 0, 0, 1, 1, 1},
  }
};

/*
  Each exit door represents the doors in each room.
  You can exit from the rooms through:
  -> up doors:    line 0 and column exit door
  -> down doors:  line 7 and column exit door
  -> right doors: line exit door and column 7
  -> left doors:  line exit door and column 0
*/
const byte exitDoorFirst = 3;
const byte exitDoorSecond = 4;

const byte roomsCommunication[roomsSize][directions] = {
  // up, down, left, right
  {2, 2, 1, 1},   // room 0
  {3, 3, 0, 0},   // room 1
  {0, 0, 3, 3},   // room 2
  {1, 1, 2, 2}    // room 3
};

/*
  Given one of the rooms, display it
*/
void setRoom(LedControl &lc, int room){
  for (int row = 0; row < matrixSize; row++) {
    for (int col = 0; col < matrixSize; col++) {
      lc.setLed(0, row, col, rooms[room][row][col]);
    }
  }
};

/*
  Reset the matrix values
*/
void resetMatrix(LedControl lc){
  for (int row = 0; row < matrixSize; row++) {
    for (int col = 0; col < matrixSize; col++) {
      lc.setLed(0, row, col, false);
    }
  }
};

#endif