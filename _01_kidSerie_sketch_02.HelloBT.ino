/*
  Project name:
     01 #kidSerie - Arduino & MIT App Instaler 2 & HC-06
     (Smart Phone App!)
     Flavour II - 
     Hex File: _01_kidSerie_sketch_02.HelloBT.ino
   Revision History:
     20161008:
       - board found on https://www.hackster.io/ Examples
      (connect everything on HC-06 to access arduino by smart phone)

   Description:
        This program lets you to control a LED on pin 13 of arduino using a bluetooth module.

   MCU:                  Arduino 1.6.12 - @16MHz       http://www.arduino.cc/
   IC Chip:              Bluetooth Datasheet - HC-06   https://goo.gl/s9ZcNC

   Connections:
      See Official youtube channel vids: https://youtu.be/u1xqW6qtFXs


   Based on:  Bluetooh Basic: LED ON OFF 
   Website:   http://bit.do/Avishkar
   Code by:   Mayoogh Girish
   Down   :   https://github.com/Mayoogh/Arduino-Bluetooth-Basic

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License v3 as published by
   the Free Software Foundation
   
*/
char btooth = 0;                       // Variable for storing received data
void setup()
{
  Serial.begin(9600);                  // Sets the baud for serial data transmission
  //Serial.begin(115200);              // I set this baudrate
  pinMode(13, OUTPUT);                 // Sets digital pin 13 as output pin
}
void loop()
{
  if (Serial.available() > 0)          // Send data only when you receive data:
  {
    btooth = Serial.read();            //Read the incoming data & store into data
    Serial.println(btooth);            //Print Value inside data in Serial monitor
    if (btooth == 'on')                // Checks whether value of data is equal to 1
      digitalWrite(13, HIGH);          //If value is 1 then LED turns ON
    else if (data == 'off')            //  Checks whether value of data is equal to 0
      digitalWrite(13, LOW);           //If value is 0 then LED turns OFF
  }
}
