#pragma once
#ifndef MENU_INPUT_H
#define MENU_INPUT_h

#include "LiquidCrystal.h"
#include "JoyStick.h"
#include "MenuDisplay.h"
#include "CustomCharacters.h"

const byte arrowPosition = 0;
const byte userInputStartPosition = 2;
const byte deletePosition = 11;
const byte verifyPosition = 13;
const byte exitPosition = 15;

const byte symbolPositions[3] = {
  deletePosition, verifyPosition, exitPosition 
};

struct MenuInput{
  // how many characters the user has completed
  byte currentInputCursorPosition;
  // how many characters overflowed the X axis 
  // for the alphabet
  byte alphabetPositionBoundary;

  byte currentCursorLinePosition;
  byte currentCursorColumnPosition;

  MenuInput(){
    currentCursorLinePosition = 1;
    currentCursorColumnPosition = 0;

    currentInputCursorPosition = 0;
    alphabetPositionBoundary = 0;
  } 

  // functions to handle user movements
  void userInputHandler(LiquidCrystal lcd, Joystick joystick, const int maxInput, char* userInput[], const char* userAlphabet[]);
  void userAlphabetHandler(LiquidCrystal lcd, Joystick joystick, const char* userAlphabet[], const int leftBoundary, const int rightBoundary);
  void userInputControlsHandler(LiquidCrystal lcd, Joystick joystick);
  int joystickPressControlsHandler(LiquidCrystal lcd, Joystick joystick, const int maxInput, char* userInput[]);
  void userCursorLineHandler(LiquidCrystal lcd, Joystick joystick, const char* userAlphabet[]);

  // function to display user input
  void displayUserInput(LiquidCrystal lcd, const char* userInput[]);

  // function to reset input variables
  void resetInputVariables();
};

/*
  Listen to the switch presses from the joystick.
  
  If the joystick switch was pressed, look into the
  current letter the user is pointing at in the alphabet,
  append it to the user input only if the maximum input size
  was not reached yet.

  Display the user input on line 1 of the LCD.
*/
void MenuInput::userInputHandler(LiquidCrystal lcd, Joystick joystick, const int maxInput, char* userInput[], const char* userAlphabet[]) {
  // display the arrow at the beggining of the input
  lcd.setCursor(arrowPosition, 0);
  lcd.write(arrowIndex);

  // display the delete symbol
  lcd.setCursor(deletePosition, 0);
  lcd.write(deleteIndex);

  // display the verify symbol 
  lcd.setCursor(verifyPosition, 0);
  lcd.write(verifyIndex);

  // display the exit symbol
  lcd.setCursor(exitPosition, 0);
  lcd.write(exitIndex);

  // if the input has not reached the maximum size
  if (currentInputCursorPosition < maxInput) {
    // check if the joystick switch was pressed, and if
    // it was, add the letter the user is pointing at in the input
    if (joystick.currentSwitchStateChanged == HIGH) {
      userInput[currentInputCursorPosition] = userAlphabet[currentCursorColumnPosition];
      currentInputCursorPosition += 1;
    }
  }
  
  displayUserInput(lcd, userInput);
};

void MenuInput::displayUserInput(LiquidCrystal lcd, const char* userInput[]){
  // display the user input
  for (int i = 0; i < currentInputCursorPosition; i++) {
    lcd.setCursor(userInputStartPosition + i, 0);
    lcd.print(userInput[i]);
  }
}

/*
  Listen to the joystick movement. Depending on
  the direction it is pointing at (left or right), 
  move from the current letter to the pointed letter.

  The current letter of the alphabet is blinking.

  If the movement goes beyond boundaries, shift the
  alphabet one position (to left or right).
*/
void MenuInput::userAlphabetHandler(LiquidCrystal lcd, Joystick joystick, const char* userAlphabet[], const int leftBoundary, const int rightBoundary) {
  // if the joystick moved to the left and the cursor position
  // will not move out of the left boundary of the alphabet
  if (joystick.direction == joystickLeft && currentCursorColumnPosition > leftBoundary) {
    currentCursorColumnPosition -= 1;

    // if the cursor position moved beyond the right boundary of the LCD 
    if (currentCursorColumnPosition < alphabetPositionBoundary) {
      alphabetPositionBoundary -= 1;
    }
  } 

  // if the joystick moved to the right and the cursor position
  // will not move out of the right boundary of the alphabet
  if (joystick.direction == joystickRight && currentCursorColumnPosition < rightBoundary) {
    currentCursorColumnPosition += 1;

    // if the cursor position moved beyond the left boundary of the LCD 
    if (currentCursorColumnPosition >= alphabetPositionBoundary + 16) {
      alphabetPositionBoundary += 1;
    }
  }

  // display the alphabet and make the element positioned at
  // the cursor position blink
  for (int i = 0; i < min(rightBoundary, 16); i++) {
    if (i + alphabetPositionBoundary == currentCursorColumnPosition) {
      // blink the cursor
      displayBlinkingChar(lcd, userAlphabet[currentCursorColumnPosition], 1, i);
    } else {
      // display the characters of the alphabet
      lcd.setCursor(i, 1);
      lcd.print(userAlphabet[i + alphabetPositionBoundary]);
    }   
  }
};

/*
  Listens to the joystick movement on the X axis 
  when the cursor is poiting to the 1st line( where the controls are).

  Depending on the cursor position, make the character
  to which is pointing at blink, at the others static.
  For any empty space, display a simple cursor.
*/
void MenuInput::userInputControlsHandler(LiquidCrystal lcd, Joystick joystick){
  if (joystick.direction == joystickLeft && currentCursorColumnPosition > 0) {
    currentCursorColumnPosition -= 1;
  }

  if (joystick.direction == joystickRight && currentCursorColumnPosition < exitPosition) {
    currentCursorColumnPosition += 1;
  }

  switch (currentCursorColumnPosition) {
    case deletePosition:
      displayBlinkingInt(lcd, deleteIndex, currentCursorLinePosition, currentCursorColumnPosition);

      lcd.setCursor(verifyPosition, currentCursorLinePosition);
      lcd.write(verifyIndex);
      
      lcd.setCursor(exitPosition, currentCursorLinePosition);
      lcd.write(exitIndex);
      break;
    
    case verifyPosition:
      displayBlinkingInt(lcd, verifyIndex, currentCursorLinePosition, currentCursorColumnPosition);
      
      lcd.setCursor(deletePosition, currentCursorLinePosition);
      lcd.write(deleteIndex);
      
      lcd.setCursor(exitPosition, currentCursorLinePosition);
      lcd.write(exitIndex);
      break;

    case exitPosition:
      displayBlinkingInt(lcd, exitIndex, currentCursorLinePosition, currentCursorColumnPosition);

      lcd.setCursor(verifyPosition, currentCursorLinePosition);
      lcd.write(verifyIndex);   

      lcd.setCursor(deletePosition, currentCursorLinePosition);
      lcd.write(deleteIndex);   
      break;

    default:
      lcd.setCursor(currentCursorColumnPosition, currentCursorLinePosition);
      lcd.cursor();
      break;
  }
};

/*
  Listens to joystick presses when the cursor
  is on the 1st line (control line).
*/
int MenuInput::joystickPressControlsHandler(LiquidCrystal lcd, Joystick joystick, const int maxInput, char* userInput[]){
  // if the joystick has not been pressed, exit
  if (joystick.currentSwitchStateChanged != HIGH) {
    return -1;
  }

  // if the cursor is not pointing to the first line, exit
  if (currentCursorLinePosition != 0) {
    return -1;
  }
  
  lcd.clear();
  
  if (currentCursorColumnPosition == deletePosition) {
    currentInputCursorPosition = 0;
    // empty the userInput values
    for (int i = 0; i < maxInput; i++) {
      userInput[i] = "";
    }
  } else if (currentCursorColumnPosition == verifyPosition) {
    // TO DO: save the name in memory and EEPROM
    return 1;
  } else if (currentCursorColumnPosition == exitPosition) {
    return 1;
  }

  return -1;
}

/*
  Listens to the joystick movements on the Y axis.

  If the joystick moved up, and the user is currently on
  the 2nd line, switch to 1st line.

  If the joysticl moved down, and the user is currently
  on the 1st line, switch to the 2nd line.
*/
void MenuInput::userCursorLineHandler(LiquidCrystal lcd, Joystick joystick, const char* userAlphabet[]){
  if (joystick.direction == joystickUp && currentCursorLinePosition == 1) {
    // when switching from the alphabet to the controls, 
    // update the column cursor to proportionally point to a LCD
    // column, not a column from the alphabet
    currentCursorColumnPosition -= alphabetPositionBoundary;

    lcd.setCursor(currentCursorColumnPosition, currentCursorLinePosition);
    lcd.print(userAlphabet[currentCursorColumnPosition + alphabetPositionBoundary]);

    // update the line position
    currentCursorLinePosition -= 1;
  }

  if (joystick.direction == joystickDown && currentCursorLinePosition == 0) {
    // before switching from a line to another, display the
    // current character to be sure that it will not be 
    // left on a blinking state
    lcd.setCursor(currentCursorColumnPosition, currentCursorLinePosition);

    switch (currentCursorColumnPosition) {
      case deletePosition:
        lcd.write(deleteIndex);
        break;      
      case verifyPosition:
        lcd.write(verifyIndex);
        break;
      case exitPosition:
        lcd.write(exitIndex);      
        break;
      default:
        break;
    }

    currentCursorLinePosition += 1;
  }
};

/*
  Reset the variables to be reusable for new
  alphabets or user inputs.
*/
void MenuInput::resetInputVariables(){
    currentCursorLinePosition = 1;
    currentCursorColumnPosition = 0;

    currentInputCursorPosition = 0;
    alphabetPositionBoundary = 0;
};


#endif