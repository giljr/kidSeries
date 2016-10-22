/*
  Project name:
     04 #kidSerie - Arduino + HC06 + MIT App Inventor 2 + Sliding  
                    Leds       (see details also in video below!)
     Flavour I -
     Hex File: _04_kidSerie_sketch_01_bluetooth_SlidingLeds.ino
   Revision History:
     20161020:
       - board found on youtube https://youtu.be/8CjL5JB6YfI
   Description:
        A Simple bluetooth app which demonstrates how to connect
        smartphone + HC06 + using arduino + MIT App Inventor 2.
        The application uses TWO sliders to change the LED's  
        brightness.
   MCU:       Arduino 1.6.12 - @16MHz       http://www.arduino.cc/
   IC Chip:   Bluetooth Datasheet - HC-06   https://goo.gl/s9ZcNC
   Connections:
      See Official youtube channel vids:
   Based on:   Control 5 leds using arduino and app inventor.
               https://youtu.be/8CjL5JB6YfI
   Code by Mohamed Aly
This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License v3 as published by the Free Software Foundation
*/
#include <SoftwareSerial.h>
SoftwareSerial bluetooth(2, 3) // Demand the lib SoftwareSerial 
int blueLed = 9;               // to get arduino's port Rx & Tx free  
                               // to debug in Serial
int orangeLed = 10;            // Use port 9 and 10 for Blue and 
                               // Orange leds
int valueBlue = 0;             // Initialize values for set the 
                               // brightness         
int valueOrange = 0;           // of each led, in this order: blue 
                               // than orange
boolean debug = true;
void setup() {
  Serial.begin(115200);        // Begin Arduino Serial
  bluetooth.begin(115200);     // in the same baudrate as bluetooth
  Serial.println("MIT App Inventor 2");  
                               // Print smartphone 
                               //software version 
  Serial.println("Release nb151 - Sep 13, 2016"); 
                               // see:   
                               // http://ai2.appinventor.mit.edu/
  pinMode(blueLed, OUTPUT);    // Set LEDs' pins as output
  pinMode(orangeLed, OUTPUT);
}
void loop() {
  if (bluetooth.available()) {
    while (bluetooth.available()) 
                               // Confirm if bluetooth are 
                               // available
    {
      valueBlue = bluetooth.parseInt();  
                               // While it is, parse   
                               // integer numbers.
      valueOrange = bluetooth.parseInt();
      if (debug) {             // Debug it into arduino serial
        Serial.print("B: ");   // Disable it if you will: set 
                               // boolean debug to false
        Serial.println(valueBlue);
        Serial.print("O: ");
        Serial.println(valueOrange);
                 }
      if (bluetooth.read() == '\n') {
        analogWrite(blueLed, valueBlue); 
                                // Finally send it to  
                                // bluetooth port
        analogWrite(orangeLed, valueOrange);
                 }
           }
      }
}
