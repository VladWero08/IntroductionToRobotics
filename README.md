# 🤖 Introduction to Robotics [2023-2024]

This repository includes homework written for the course entitled **Introduction to Robotics**, taken in my _3rd year_ at the _Faculty of Mathematics and Computer Science, University of Bucharest_. These assignments aimed to control an Arduino Uno board [^1] [^2], to gain a deeper understanding of how to control hardware components with software. 

Every single homework will have its description, references, implementation details, and source code. 

#### Table of contents
- [H02 - RGB LED controlled by potentiometers](#homework-02-rgb-led-controlled-by-potentiometers)

## Homework 02 - RGB LED controlled by potentiometers
The focus of this assignment was to control each color channel(_Red_, _Green_, _Blue_) of an RGB LED [^3] using individual potentiometers. 

I used exactly **3 potentiometers** to control each channel. Also, I have mapped each potentiometer's value to an RGB valid value with which I have modified the color channel intensity. This was the Arduino setup:

| From the top | From the side|
| ---------- | ---------- |
| ![circuit01](https://github.com/VladWero08/IntroductionToRobotics/assets/77508081/fa89d0c7-f157-47c2-8284-5bd2edd08d8d) | ![circuit02](https://github.com/VladWero08/IntroductionToRobotics/assets/77508081/dac2c372-9510-41cc-987e-60f4b6065b7a) | 

Check out the <a href="https://youtube.com/shorts/dFWHFmvstL8?feature=share" target="_blank">demo</a>.

## Homework 03 - Elevator wannabe
This assignment aimed  to simulate _a 3-floor elevator control system_ using LEDs and buttons. Also, to get more familiar with debouncing [^4] techniques and coordinating multiple components.

I used **a red LED and a pushbutton** for each of the elevator's level. Besides this, I added a **green LED** that is blinking during the elevator movement.
If the elevator is moving and another pushbutton is pressed, schedule the event in a _waiting queue_. ( source code for the queue implementation can be find [here](https://github.com/sdesalas/Arduino-Queue.h) )

| From the top | From the side|
| ---------- | ---------- |
| ![circuit_h3_top](https://github.com/VladWero08/IntroductionToRobotics/assets/77508081/65e59c57-b88b-4305-ae58-7666abb763f7) |![circuit_h3_side](https://github.com/VladWero08/IntroductionToRobotics/assets/77508081/2e3b1604-4ef2-4801-b4eb-780360071633) |

Check out the <a href="https://youtube.com/shorts/Dna8v8pamGY" target="_blank">demo</a>. [WITHOUT waiting queue]

Check out the 2nd <a href="https://youtube.com/shorts/a4mvA3g6lFc?feature=share" targel="_blank">demo</a> [WITH waiting queue].

[^1]: _Introduction to Arduino_ : https://www.arduino.cc/en/Guide/Introduction
[^2]: _What is Arduino Uno?_ : https://store.arduino.cc/products/arduino-uno-rev3
[^3]: _How does an RGB LED work?_ : https://www.circuitbread.com/tutorials/how-rgb-leds-work-and-how-to-control-color
[^4]: _Debounce on a pushbutton_ : https://docs.arduino.cc/built-in-examples/digital/Debounce
