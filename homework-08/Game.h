#pragma once
#ifndef GAME_H
#define GAME_H

#include <LiquidCrystal.h>
#include <LedControl.h>

#include "CustomCharacters.h"
#include "MenuDisplay.h"
#include "JoyStick.h"
#include "Player.h"
#include "Note.h"
#include "Utils.h"

const byte heartsStartPosition = 13;

struct Game{
  // time since the game started
  unsigned int time;
  // last time when the time was incremented
  unsigned long lastTimeIncrement;
  byte notes;
  byte lives;

  bool gameIsRunning = true;
  unsigned long gameEndingTime = 0;
  byte gameEndedMenuArrow = 0; 

  LedControl lc;
  Player player;
  Note note;

  Game(LedControl &lc): time(0), notes(0), lives(3), lc(lc), player(this->lc){
    lastTimeIncrement = millis();
  }

  // functions to control the state of game
  void checkPlayerFoundNote();
  void checkPlayerWin(LiquidCrystal lcd);

  // functions to display the game on the LCD
  int displayMenu(LiquidCrystal lcd, Joystick joystick);

  void displayGameRunningMenu(LiquidCrystal lcd);
  void displayLives(LiquidCrystal lcd);
  void displayNotes(LiquidCrystal lcd);
  void displayTime(LiquidCrystal lcd);
  
  int displayGameEndedMenu(LiquidCrystal lcd, Joystick joystick);
  int gameEndedMenuHandler(LiquidCrystal lcd, Joystick joystick);
  void displayGameEndedMessage(LiquidCrystal lcd);
  
  // function to reset the game
  void resetGame();
};

void Game::checkPlayerFoundNote(){
  // if the player and the note are in different rooms, exit
  if (player.currentRoom != note.currentRoom)
    return;

  // if the player and the note are not on the same position, exit
  if (player.row != note.row || player.column != note.column) 
    return;

  // if this section of the function was reached, it means
  // that the player has found the note
  notes += 1;

  // if the player already collected 3 notes,
  // make sure the note spawns in a different room 
  // from the room the player is currently in
  if (notes >= 3) {
    note.spawnNoteDifferentRoom(player.currentRoom);
  } 
  // otherwise, just spawn the note randomly
  else {
    note.spawnNoteRandomly();
  }
}

void Game::checkPlayerWin(LiquidCrystal lcd){
  // if the number of notes reached 5, it means the player won
  if (notes == 1) {
    resetMatrix(lc);
    lcd.clear();
    gameEndingTime = millis();
    gameIsRunning = false;
  }
}


/*
  Display the whole game menu
*/
int Game::displayMenu(LiquidCrystal lcd, Joystick joystick){
  if (gameIsRunning) {
    displayGameRunningMenu(lcd);
    
    // listents to the position change of the player
    player.positionWatcher(lc, joystick);

    // check if the player found any notes
    checkPlayerFoundNote();
    // check if the player found enough notes
    checkPlayerWin(lcd);

    return -1;
  } else {
    return displayGameEndedMenu(lcd, joystick);
  }
};

void Game::displayGameRunningMenu(LiquidCrystal lcd){
  displayLives(lcd);
  displayNotes(lcd);
  displayTime(lcd);

  player.displayPlayer(lc);
  note.displayNote(lc, player);
};

void Game::displayLives(LiquidCrystal lcd){
  for(int i = 0; i < lives; i++) {
    lcd.setCursor(heartsStartPosition + i, 0);
    lcd.write(skullIndex);
  }
};

void Game::displayNotes(LiquidCrystal lcd){
  lcd.setCursor(0, 0);
  lcd.print("Notes: ");

  // 7 is the length of "Notes "
  lcd.setCursor(7, 0);
  lcd.print(notes);
};

void Game::displayTime(LiquidCrystal lcd){
  // if a 1000 ms have passed, it means a second,
  // so the time will be incremented by 1
  if ((millis() - lastTimeIncrement) >= 1000) {
    lastTimeIncrement = millis();
    time += 1;
  }

  lcd.setCursor(0, 1);
  lcd.print("Time: ");
  
  displayTimeFromSeconds(lcd, time, 6, 1);
};



int Game::displayGameEndedMenu(LiquidCrystal lcd, Joystick joystick){
  if ((millis() - gameEndingTime) < 3000) {
    displayGameEndedMessage(lcd);
  } else if ((millis() - gameEndingTime) >= 3000 && (millis() - gameEndingTime) <= 3500) {
    lcd.clear();
  } else {
    lcd.setCursor(0, gameEndedMenuArrow);
    lcd.write(arrowIndex);

    lcd.setCursor(2, 0);
    lcd.print("Play again");

    lcd.setCursor(2, 1);
    lcd.print("Back");

    return gameEndedMenuHandler(lcd, joystick);
  }

  return -1;
}

void Game::displayGameEndedMessage(LiquidCrystal lcd){
  displayMessageInCenter(lcd, "You escaped!", 0);
  displayTimeFromSeconds(lcd, time, 5, 1);
}

int Game::gameEndedMenuHandler(LiquidCrystal lcd, Joystick joystick){
  if (joystick.direction == joystickUp && gameEndedMenuArrow == 1) {
    lcd.setCursor(0, gameEndedMenuArrow);
    lcd.write(" ");
    
    gameEndedMenuArrow -= 1;
  }

  if (joystick.direction == joystickDown && gameEndedMenuArrow == 0) {
    lcd.setCursor(0, gameEndedMenuArrow);
    lcd.write(" ");

    gameEndedMenuArrow += 1;
  }

  if (joystick.currentSwitchStateChanged == HIGH) {
    lcd.clear();
    resetGame();

    if (gameEndedMenuArrow == 0) {
      // start the game again and enter the game menu
      gameIsRunning = true;
      return 11;
    } else {
      // go back to the main menu
      return 1;
    }
  }

  return -1;
}



void Game::resetGame(){
  time = 0;
  notes = 0;
  lives = 3;
  lastTimeIncrement = millis();

  player.initPlayer(lc);
  note.spawnNoteRandomly();
};

#endif