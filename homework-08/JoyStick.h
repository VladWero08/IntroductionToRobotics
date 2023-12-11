#pragma once
#ifndef JOYSTICK_H
#define JOYSTICK_H

/* each direction of the joystick will have a
column in the matrix associated*/
const byte joystickUp = 0;
const byte joystickDown = 1;
const byte joystickLeft = 2;
const byte joystickRight = 3;

/* joystick bounds that will identify in which
direction is the user pointing at: up, down, left, right */
const int joystickUpBound = 768;                    // 512 + 256
const int joystickDownBound = 256;                  // 512 - 256
const int joystickRightBound = 768;                 // 512 + 256
const int joystickLeftBound = 256;                  // 512 - 256
const int joystickMiddleInterval[2] = {492, 532};   // 512 - 20, 512 + 20

const byte joystickLastDirectionInterval = 500;
const byte joystickSwitchDebounceInterval = 100;

struct Joystick {
  byte pinSW;
  byte pinX;
  byte pinY;

  byte direction;
  int axisValueX, axisValueY;
  
  bool currentSwitchState;
  bool currentSwitchStateChanged; 
  bool lastSwitchState; 

  unsigned long int switchDebounceTime;
  unsigned long int lastDirectionChange;

  Joystick(byte pinSW, byte pinX, byte pinY): pinSW(pinSW), pinX(pinX), pinY(pinY){
    this->currentSwitchState = LOW;
    this->currentSwitchStateChanged = LOW;
    this->lastSwitchState = LOW; 
     
    this->switchDebounceTime = 0;
    this->lastDirectionChange = 0;
  } 

  void switchHandler();
  void movementHandler();
};

void Joystick::switchHandler(){
  bool switchState = !digitalRead(pinSW);
  currentSwitchStateChanged = LOW;

  if (switchState != lastSwitchState) {
    // if the state has changed, update the last debounce time of the button
    switchDebounceTime = millis();
  } 

  if ((millis() - switchDebounceTime) > joystickSwitchDebounceInterval) {
    /* because it passed a certain debounce time, it means that the reading state
    is not a noise, so the current state of the button must be changed*/
    if (currentSwitchState != switchState) {
      currentSwitchState = switchState;    

      if (currentSwitchState == HIGH) {
        currentSwitchStateChanged = HIGH;
      }
    }
  } 

  lastSwitchState = switchState;
}

void Joystick::movementHandler(){
  if ((millis() - lastDirectionChange) <= joystickLastDirectionInterval) {
    /*if the minimum interval of time betweens joystick direction changes
    has not passed, do not evaluate the joystick's state*/
    direction = -1;
    return ;
  }

  lastDirectionChange = millis();

  axisValueX = analogRead(pinX);
  axisValueY = analogRead(pinY);

  // check in which direction is the joystick pointing at
  if (axisValueX > joystickUpBound && joystickMiddleInterval[0] < axisValueY < joystickMiddleInterval[1]) {
    direction = joystickUp;        // joystick is pointing up
  } else if (axisValueY > joystickRightBound && joystickMiddleInterval[0] < axisValueX < joystickMiddleInterval[1]) {
    direction = joystickRight;     // joystick is pointing right
  } else if (axisValueX < joystickDownBound && joystickMiddleInterval[0] < axisValueY < joystickMiddleInterval[1]) {
    direction = joystickDown;      // joystick is pointing down
  } else if (axisValueY < joystickLeftBound && joystickMiddleInterval[0] < axisValueX < joystickMiddleInterval[1]) {
    direction = joystickLeft;      // joystick is pointing left 
  }
};

#endif