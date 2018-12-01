/*
  Kids-Serie#10
  Project : Arduino Start Up


  Ino File: _10_ControllingServo_04.ino

  Flavour 04

  Date: Nov 2018

  Description: While I could spend hours learning how they work and writing the code for
               an Arduino to control it it's much faster to include the Arduino servo
               library instead; so now with only a few changes I can convert the 
               potentiometer position into motorized motion \o/

  Board: Arduino UNO

  Arduino IDE: 1.8.7

  Connections: See Fritzing in Jungletronics:

  Credits: This sketch was in public domain.

  Based on: Collin's Lab: Arduino
            https://youtu.be/pnf8ojsK6S4

  Terms of Use:  This program is free software: you can redistribute it and/or modify
                 it under the terms of the GNU General Public License v3 as published by
                 the Free Softw are Foundation - There is no guarantee whatsoever :)
                 Use at your own risk!
*/

#include <Servo.h>

Servo myServo;

void setup() {                   // put your setup code here, to run once:

  myServo.attach(9);  

}

void loop() {                    // put your main code here, to run repeatedly:

  int potValue = analogRead(0);  // readings from the potentiometer and transform 
  potValue = map(potValue, 0, 1023, 0, 180); 
  myServo.write(potValue);       // the number so the servo turns 180º 
  delay(15);
  
}
