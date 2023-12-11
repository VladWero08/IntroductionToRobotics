#pragma once
#ifndef PLAYER_H
#define PLAYER_H

#include <LedControl.h>
#include "JoyStick.h"
#include "Rooms.h"

// interval in ms between player's position blinking
const byte playerBlinkingInterval = 50;

struct Player{
  byte row;
  byte column;
  byte currentRoom;

  // controls the player's visibility
  bool isActive;
  // last time when isActive changes its states, in ms
  unsigned long lastBlinking;

  Player(LedControl &lc){
    initPlayer(lc);
  };

  // functions to handle player's movement
  void positionWatcher(LedControl &lc, Joystick joystick);
  void movementUpHandler(LedControl &lc);
  void movementDownHandler(LedControl &lc);
  void movementLeftHandler(LedControl &lc);
  void movementRightHandler(LedControl &lc);

  // function to display the player in the room
  void displayPlayer(LedControl &lc);

  // function to initate the players position;
  void initPlayer(LedControl &lc);
};

/*
  Listens to the joystick movement, and
  depending on the direction, handle that movement.
*/
void Player::positionWatcher(LedControl &lc, Joystick joystick){
  if (joystick.direction == joystickUp) {
    movementUpHandler(lc);
  }

  if (joystick.direction == joystickDown) {
    movementDownHandler(lc);
  }

  if (joystick.direction == joystickLeft) {
    movementLeftHandler(lc);
  }

  if (joystick.direction == joystickRight) {
    movementRightHandler(lc);
  }
};

void Player::movementUpHandler(LedControl &lc){
  if (row > 0) {
    // if the player moves into a wall, 
    // ignore the movement
    if (rooms[currentRoom][row - 1][column] == true) {
      return;
    }

    // unset the current position
    lc.setLed(0, row, column, false);
    // move up
    row -= 1;
  } 
  // if the player is already on the first row
  // and tries to move up, it means it wants to access a door
  else if (row == 0) {  
    // move to the next room according to 
    // the movement matrix
    currentRoom = roomsCommunication[currentRoom][joystickUp];
    setRoom(lc, currentRoom);

    row = matrixSize - 1;
  }
};

void Player::movementDownHandler(LedControl &lc){
  if (row < matrixSize - 1) {
    // if the player moves into a wall, 
    // ignore the movement
    if (rooms[currentRoom][row + 1][column] == true) {
      return;
    }

    // unset the current position
    lc.setLed(0, row, column, false);
    // move down
    row += 1;
  } 
  // if the player is already on the last row
  // and tries to move down, it means it wants to access a door
  else if(row == matrixSize - 1) {

    // move to the next room according to 
    // the movement matrix
    currentRoom = roomsCommunication[currentRoom][joystickDown];
    setRoom(lc, currentRoom);
    
    row = 0;
  }
};

void Player::movementLeftHandler(LedControl &lc){
  if (column > 0) {
    // if the player moves into a wall, 
    // ignore the movement
    if (rooms[currentRoom][row][column - 1] == true) {
      return;
    }
    
    // unset the current position
    lc.setLed(0, row, column, false);
    // move to the left
    column -= 1;
  } 
  // if the player is already on the first column
  // and tries to move to the left, it means it wants to access a door
  else if (column == 0) {
    
    // move to the next room according to 
    // the movement matrix
    currentRoom = roomsCommunication[currentRoom][joystickLeft];
    setRoom(lc, currentRoom);
  
    column = matrixSize - 1;
  }
};

void Player::movementRightHandler(LedControl &lc){
  if (column < matrixSize - 1) {
    // if the player moves into a wall, 
    // ignore the movement
    if (rooms[currentRoom][row][column + 1] == true) {
      return;
    }

    // unset the current position
    lc.setLed(0, row, column, false);
    // move to the right
    column += 1;
  } 
  // if the player is already on the first column
  // and tries to move to the left, it means it wants to access a door
  else if (column == matrixSize - 1) {

    // move to the next room according to 
    // the movement matrix
    currentRoom = roomsCommunication[currentRoom][joystickRight];
    setRoom(lc, currentRoom);

    column = 0;
  }
};

/*
  Display the current position of the player 
  in the room.
*/
void Player::displayPlayer(LedControl &lc){
  if ((millis() - lastBlinking) > playerBlinkingInterval) {
    lastBlinking = millis();
    isActive = !isActive;
    lc.setLed(0, row, column, isActive);
  }
};

void Player::initPlayer(LedControl &lc){
  row = 1;
  column = 1;
  isActive = true;
  currentRoom = random(0, roomsSize);
  setRoom(lc, currentRoom);
}

#endif