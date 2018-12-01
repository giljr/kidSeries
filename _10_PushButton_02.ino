
/*
  Kids-Serie#10
  Project : Arduino Start Up


  Ino File: _10_PushButton_02.ino

  Flavour 02

  Date: Nov 2018

  Description:  pressing the button lights the led according to the presence
                or not of 5 volts - digital mode :)

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

void setup() {                    // put your setup code here, to run once:
  
  pinMode(3, OUTPUT);             // LED connected

  pinMode(7, INPUT);              // Push Button connected

}

void loop() {                     // put your main code here, to run repeatedly:
  
  if (digitalRead(7) == HIGH) {   // is button pressed?

    digitalWrite(3, HIGH);        // yes, then light LED

  } else {

    digitalWrite(3, LOW);         // no, turn off the led ;)

  }

}
