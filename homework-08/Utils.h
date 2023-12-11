#pragma once
#ifndef UTILS_H
#define UTILS_H

#include <LiquidCrystal.h>

/*
  Given an array of chars, concatenate (from index 0 to nth) 
  them to form a number. If the number is from the [0, 255] interval,
  transform it to a byte and update the targetVariable.
*/
bool saveCharAsByte(char* numberChar[], int numberSize, byte &targetVariable){
  String numberString;
  int numberInt;

  // concatenate the array of chars into a String
  for (int i = 0; i < numberSize; i++) {
    numberString += numberChar[i];
  }

  // transform the char number into a int
  numberInt = numberString.toInt();

  // exit if its not from the interval [0, 255]
  if (numberInt < 0 || numberInt > 255) {
    return false;
  }

  // update the given variable
  targetVariable = (byte) numberInt;
  
  return true;
};

/*
  Given an array of chars, concatenate (from index 0 to nth) 
  them to form a number. If the number is from the [0, 15] interval,
  transform it to a byte and update the targetVariable.
*/
bool saveCharAsByteMatrix(char* numberChar[], int numberSize, byte &targetVariable){
  String numberString;
  int numberInt;

  // concatenate the array of chars into a String
  for (int i = 0; i < numberSize; i++) {
    numberString += numberChar[i];
  }

  // transform the char number into a int
  numberInt = numberString.toInt();
  Serial.println(numberInt);

  // exit if its not from the interval [0, 15]
  if (numberInt < 0 || numberInt > 15) {
    return false;
  }

  // update the given variable
  targetVariable = (byte) numberInt;
  
  return true;
};

#endif