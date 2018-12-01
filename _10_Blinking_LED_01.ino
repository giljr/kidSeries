
/*
  Kids-Serie#10
  Project : Arduino Start Up


  Ino File: _10_Blinking_LED_01.ino

  Flavour 01

  Date: Nov 2018

  Description:  Let's say I want to flash an LED connected to Pin 3


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

void setup() {
                            // put your setup code here, to run once:
  pinMode(3, OUTPUT);

}

void loop() {
                            // put your main code here, to run repeatedly:
  digitalWrite(3, HIGH);

  delay(500);

  digitalWrite(3, LOW);

  delay(500);

}
