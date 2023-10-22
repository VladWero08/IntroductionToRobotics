# 🤖 Introduction to Robotics [2023-2024]

This repository includes homework written for the course entitled **Introduction to Robotics**, taken in my _3rd year_ at the _Faculty of Mathematics and Computer Science, University of Bucharest_. These assignments aimed to control an Arduino Uno board [^1] [^2], to gain a deeper understanding of how to control hardware components with software. 

Every single homework will have its description, references, implementation details, and source code. 

#### Table of contents
- [H02 - RGB LED controlled by potentiometers](#homework-02)

## Homework 02 - RGB LED controlled by potentiometers
The focus of this assignment was to control each color channel(_Red_, _Green_, _Blue_) of an RGB LED [^3] using individual potentiometers. 

I used exactly **3 potentiometers** to control each channel. Also, I have mapped each potentiometer's value to an RGB valid value with which I have modified the color channel intensity. This was the Arduino setup:

<div style="display: flex">

| From the top | From the side|
| ---------- | ---------- |
| ![circuit01](https://github.com/VladWero08/IntroductionToRobotics/assets/77508081/fa89d0c7-f157-47c2-8284-5bd2edd08d8d) | ![circuit02](https://github.com/VladWero08/IntroductionToRobotics/assets/77508081/dac2c372-9510-41cc-987e-60f4b6065b7a) | 

Check out the [demo](https://youtube.com/shorts/dFWHFmvstL8?feature=share).

## Homework 03


[^1]: _Introduction to Arduino_ : https://www.arduino.cc/en/Guide/Introduction
[^2]: _What is Arduino Uno?_ : https://store.arduino.cc/products/arduino-uno-rev3
[^3]: _How does an RGB LED work?_ : https://www.circuitbread.com/tutorials/how-rgb-leds-work-and-how-to-control-color
