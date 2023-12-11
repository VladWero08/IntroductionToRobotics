#pragma once
#ifndef NOTE_H
#define NOTE_H

#include <LedControl.h>
#include "Player.h"
#include "Rooms.h"

const int noteActiveBlinkingInterval = 500;
const byte noteInactiveBlinkingInterval = 100;

struct Note{
  byte row;
  byte column;
  byte currentRoom;

  // controls the note's visibility
  bool isActive;
  // last time when isActive changes its states, in ms
  unsigned long lastBlinking;

  Note(){
    spawnNoteRandomly();
  }

  // functions to spawn the note
  void spawnNoteRandomly();
  void spawnNoteDifferentRoom(const byte playerRoom);

  // function to generate a random position in a room
  void generatePositionInRoom();

  // function to display the note in the room
  void displayNote(LedControl &lc, Player player);
};

/*
  Spawn a note in a random room,
  in a random, but valid position in that room
*/
void Note::spawnNoteRandomly(){
  currentRoom = random(0, roomsSize);  
  generatePositionInRoom();
};

/*
  Spawn the note in a different room
  from the room the player is currently in
*/
void Note::spawnNoteDifferentRoom(const byte playerRoom){
  currentRoom = random(0, roomsSize);

  while (currentRoom == playerRoom) {
    currentRoom = random(0, roomsSize);
  }

  generatePositionInRoom();
};

/*
  Generate a position in the currentRoom
  until it is a valid one
*/
void Note::generatePositionInRoom(){
  row = random(0, matrixSize);
  column = random(0, matrixSize);

  // while the position generated is invalid,
  // generate new positions and check their validity
  while (rooms[currentRoom][row][column] == true){
    row = random(0, matrixSize);
    column = random(0, matrixSize);
  }
};

/*
  Display the current position of the note 
  in the room.
*/
void Note::displayNote(LedControl &lc, Player player){
  // if the note and the player are placed in different rooms, exit
  if (currentRoom != player.currentRoom)
    return;

  // depending on the state of the note,
  // check if the state should be toggled 
  if (isActive) {
    if ((millis() - noteActiveBlinkingInterval) > lastBlinking) {
      lastBlinking = millis();
      isActive = !isActive;
      lc.setLed(0, row, column, isActive);
    }
  } else {
    if ((millis() - noteInactiveBlinkingInterval) > lastBlinking) {
      lastBlinking = millis();
      isActive = !isActive;
      lc.setLed(0, row, column, isActive);
    }
  }
};

#endif