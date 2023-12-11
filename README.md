# 🤖 Introduction to Robotics [2023-2024]

This repository includes homework written for the course entitled **Introduction to Robotics**, taken in my _3rd year_ at the _Faculty of Mathematics and Computer Science, University of Bucharest_. These assignments aimed to control an Arduino Uno board [^1] [^2], to gain a deeper understanding of how to control hardware components with software. 

Every single homework will have its description, references, implementation details, and source code. 

#### Table of contents
- [H02 - RGB LED controlled by potentiometers](#homework-02)
- [H03 - Elevator wannabe](#homework-03)
- [H04 - 7-segment display controlled by a joystick](#homework-04)
- [H05 - Stopwatch timer](#homework-05)
- [H06 - Pseudo-smart environment monitor and logger](#homework-06)
- [H08 - Mini Sinister Escape: a matrix game](#homework-08)

## Homework 02 - RGB LED controlled by potentiometers
The focus of this assignment was to control each color channel(_Red_, _Green_, _Blue_) of an RGB LED [^3] using individual potentiometers. 

I used exactly **3 potentiometers** to control each channel. Also, I have mapped each potentiometer's value to an RGB valid value with which I have modified the color channel intensity. This was the Arduino setup:

| From the top | From the side|
| ---------- | ---------- |
| ![circuit01](https://github.com/VladWero08/IntroductionToRobotics/assets/77508081/fa89d0c7-f157-47c2-8284-5bd2edd08d8d) | ![circuit02](https://github.com/VladWero08/IntroductionToRobotics/assets/77508081/dac2c372-9510-41cc-987e-60f4b6065b7a) | 

Check out the <a href="https://youtube.com/watch/dFWHFmvstL8?feature=share" target="_blank">demo</a>.

## Homework 03 - Elevator wannabe
This assignment aimed to simulate _a 3-floor elevator control system_ using LEDs and buttons. Also, to get more familiar with debouncing [^4] techniques and coordinating multiple components.

I used **a red LED and a pushbutton** for each of the elevator's level. Besides this, I added a **green LED** that is blinking during the elevator movement.
If the elevator is moving and another pushbutton is pressed, schedule the event in a _waiting queue_. ( source code for the queue implementation can be find [here](https://github.com/sdesalas/Arduino-Queue.h) )

| From the top | From the side|
| ---------- | ---------- |
| ![circuit_h3_top](https://github.com/VladWero08/IntroductionToRobotics/assets/77508081/65e59c57-b88b-4305-ae58-7666abb763f7) |![circuit_h3_side](https://github.com/VladWero08/IntroductionToRobotics/assets/77508081/2e3b1604-4ef2-4801-b4eb-780360071633) |

Check out the <a href="https://youtube.com/watch/Dna8v8pamGY" target="_blank">demo</a>. [WITHOUT waiting queue]

Check out the 2nd <a href="https://youtube.com/watch/a4mvA3g6lFc?feature=share" targel="_blank">demo</a>. [WITH waiting queue]

## Homework 04 - 7-segment display controlled by a joystick
The focus for this assignment to control the position of the 7-segment display [^5] and to _draw_ on the display. The movement between segments is natural, meaning that they jump only to their neighbours.

After the _switch button_ on the joystick is pressed, the current segment will change its state (ON -> OFF, OFF -> ON). To reset all the segments, press the switch button for 2 seconds.

| From the top | From the side|
| ---------- | ---------- |
| ![circuit_h4_top](https://github.com/VladWero08/IntroductionToRobotics/assets/77508081/917d96ae-88f7-4547-8046-e60cd0960568) | ![circuit_h4_side](https://github.com/VladWero08/IntroductionToRobotics/assets/77508081/aff6561a-888f-4102-8a02-0371cb09ddd0) |

Check out the <a href="https://youtube.com/watch/6vFm29PpfPI">demo</a>. 

## Homework 05 - Stopwatch timer
The aim of this assignment was to simulate a stopwatch timer using a _4 digit 7 segment display_ [^6] and _3 buttons_. The buttons will have the following functionalities: 
- button 1: start / pause
- button 2: reset the stopwatch ( if in _PAUSE_ mode), reset the saved laps ( if in _lap viewing mode_)
- button 3: save lap ( if in _RUNNING_ mode ), cycle through last saved laps ( _maximum 4 laps_ )

Whenever pressing the **lap button (2nd)**, while the stopwatch is _running_, the current time will be saved in a list with all _saved laps_. If more than 4 laps will be saved, the previous laps will be overridden. 
When pressing the **reset button (3rd)**, while the stopwatch is _paused_, the stopwatch will go back to _000.0_; respectively, while _displaying the saved laps_, all the saved laps will be deleted _and_ the stopwatch will be reset.

| From the top | From the side|
| ---------- | ---------- |
| ![circuit_h5_top](https://github.com/VladWero08/IntroductionToRobotics/assets/77508081/e5f5ffe9-6352-464e-8eca-9773a5b34dca) | ![circuit_h5_side](https://github.com/VladWero08/IntroductionToRobotics/assets/77508081/56d57b78-5e2e-40b2-9deb-694b13d9462d) | 

Check out the <a href="https://www.youtube.com/watch?v=1QCtBLAy9iM">demo</a>. 

## Homework 06 - Pseudo-smart environment monitor and logger
A system that monitors an ultrasonic (**HC-SR04**[^7]) and **photocell (LDR)** sensor to gather information about their state. The data gathered will be logged in EEPROM [^9][^10] memory. Sensors' values can be interrogated in real time, _or_ the last 10(**this number can be modified**) logs stored in the EEPROM memory.

For each sensor, a _threshold_ can be set. If the _RGB LED automatic mode_ is set to _ON_, it
will let the user know if the thresholds for the sensors are respected by lighting it _green_; otherwise, the light will be _red_. 

Also, the _RGB LED_ can be set to a certain color by writing the value for each color channel( R, G, B).

| From the top | From the side|
| ---------- | ---------- |
| ![h6-circuit-from-top](https://github.com/VladWero08/IntroductionToRobotics/assets/77508081/c4b7be88-9fcd-4eaf-9bdf-61e164c46db9) | ![h6-circuit-from-side](https://github.com/VladWero08/IntroductionToRobotics/assets/77508081/d7d4d809-2c71-4c26-a556-8483545f9621) |

## Homework 08 - Mini Sinister Escape: a matrix game
Mini Sinister Escape is a matrix game that is about traveling between **4 rooms** and finding notes, to escape from the house. The main goal is to find the notes **as fast as possible**. The rooms will be displayed on a **8x8 1088AS matrix**[^11], as well as the player and the notes, and the movement of the player will be controlled with a **joystick**.  Before starting to play, the player will interact with a menu, which will be displayed on a **Liquid Crystal Display (LCD)**[^12].

During the game, *a timer* and *the number of notes* collected will be displayed, so you better start looking for them.

In the matrix, you will be represented by a *fast-blinking red dot*, while the notes will be *slow-blinking red dots*. Besides this, the walls will be *static red dots*. As you might have guessed already, you will not be able to move through the walls, you need to move through *uncolored dots*.

The notes will appear randomly in one of the rooms. Similarly to the notes, at the beginning, you will be spawned in a random room. This is *how the rooms will be connected*:

<p align="center">
  <img src="https://github.com/VladWero08/IntroductionToRobotics/assets/77508081/9f16bf79-13df-4e8c-9e90-a6b28789008b" width="576" height="496"/>
</p>

So, in each room you will be able to move: *up*, *down*, *left* or *right*. This is the configuration of the rooms:

<p align="center">
  <img src="https://github.com/VladWero08/IntroductionToRobotics/assets/77508081/2cb3bc30-e8c1-49db-a001-2d9f9b466e99" width="576" height="496"/>
</p>

To move from one room to another, you will need to go to the edges of the matrix and point the joystick in the specific direction of the *"door"* (empty dots on the edge of the matrix).

#### Menu structure:
- **Main**
  - **Start game**: after the game has finished, the user will need to choose from
    1. Play again
    2. Back (to the main menu)
  - **Settings**
    1. Enter name
    2. LCD brightness setting
    3. Matrix brightness setting
    4. Sound setting (ON/OFF)
    5. Back (to the main menu)
  - **About**: description of the game

| From the top | From the side|
| ---------- | ---------- |
|  |  |

Check out the <a href="#">demo</a>. 

[^1]: _Introduction to Arduino_ : https://www.arduino.cc/en/Guide/Introduction
[^2]: _What is Arduino Uno?_ : https://store.arduino.cc/products/arduino-uno-rev3
[^3]: _How does an RGB LED work?_ : https://www.circuitbread.com/tutorials/how-rgb-leds-work-and-how-to-control-color
[^4]: _Debounce on a pushbutton_ : https://docs.arduino.cc/built-in-examples/digital/Debounce
[^5]: _7-segment display_: https://en.wikipedia.org/wiki/Seven-segment_display
[^6]: _4-digit 7 segment display_: https://softwareparticles.com/learn-how-a-4-digit-7-segment-led-display-works-and-how-to-control-it-using-an-arduino/
[^7]: _HC-SR04 sensor_: https://www.seeedstudio.com/blog/2019/11/04/hc-sr04-features-arduino-raspberrypi-guide/
[^8]: _Photocell (LDR) sensor_: https://en.wikipedia.org/wiki/Photoresistor
[^9]: _EEPROM memory in general_: https://en.wikipedia.org/wiki/EEPROM
[^10]: _EEPROM.h in Arduino_: https://docs.arduino.cc/learn/built-in-libraries/eeprom
[^11]: _How to use an 8x8 1088AS?_ : https://www.youtube.com/watch?v=X9tsfOeYnAU
[^12]: _What is LCD?_ : https://www.techtarget.com/whatis/definition/LCD-liquid-crystal-display
