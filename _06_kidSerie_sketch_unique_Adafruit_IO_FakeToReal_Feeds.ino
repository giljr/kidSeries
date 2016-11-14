/*
  Project name:
     06 #kidSerie - Go code? Explainning My Approach As Hobbyist!!!
     (code by  Mick Barela from Adafruit Industries)
     Flavour I - IV
     Hex File: _06_kidSerie_sketch_unique_Adafruit_IO_FakeToReal_Feeds.ino
   Revision History:
     20161029:
       - from github adafruit/Adafruit_MQTT_Library
  https://github.com/adafruit/Adafruit_MQTT_Library/blob/master/examples/mqtt_esp8266/mqtt_esp8266.ino
  https://learn.adafruit.com/mqtt-adafruit-io-and-you/intro-to-adafruit-mqtt
   Description:
     How to teach young people to program? I often do not write codes from scratch. I accomplish tasks taking
     pieces of code from excellent programmers and I'll assemble the puzzle pieces up the road to success.
     This is my approach how I pretend to be a programmer...This journey will help us to understand how to send
     temperature and humidity data to the Adafruit-IO Broker through the MQTT protocol using ESP8266 module.
     It's quite a task, right?
     How-to-use-it: Go to this link and click the history button to understand step-by-step. Thank you so much!! 
     https://github.com/giljr/kidSeries/blob/master/_06_kidSerie_sketch_01_04.Adafruit_IO_FakeToReal_Feeds.ino
     
     
   MCU:                  Arduino 1.6.12 - @16MHz       http://www.arduino.cc/
   ESP8266:              ESP-01                        https://goo.gl/E0Eq4l
   DHT11:                PRODUCT ID: 386               https://www.adafruit.com/product/386
   Connections:
      See Official youtube channel vids: https://youtu.be/cwYix_Oyui4
   Based on:   Mick Barela for Adafruit Industries - see below!
   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License v3 as published by
   the Free Software Foundation
*/
/* DHTServer - ESP8266 Webserver with a DHT sensor as an input
   Based on ESP8266Webserver, DHTexample, and BlinkWithoutDelay (thank you)
   Version 1.0  5/3/2014  Version 1.0   Mike Barela / Tony DiCola from Adafruit.
*/
/******************************* Esp8266**************************************/
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
/******************************* DHT Config **********************************/
#include <DHT.h>
#define DHTTYPE DHT11
#define DHTPIN  2

/************************* WiFi Access Point *********************************/

const char* ssid     = "YourRouterSSID";
const char* password = "YourRouterPassword";

ESP8266WebServer server(80);

/*************************** DHT Init   **************************************/
// Initialize DHT sensor
// NOTE: For working with a faster than ATmega328p 16 MHz Arduino chip, like an ESP8266,
// you need to increase the threshold for cycle counts considered a 1 or 0.
// You can do this by passing a 3rd parameter for this threshold.  It's a bit
// of fiddling to find the right value, but in general the faster the CPU the
// higher the value.  The default for a 16mhz AVR is a value of 6.  For an
// Arduino Due that runs at 84mhz a value of 30 works.
// This is for the ESP8266 processor on ESP-01
DHT dht(DHTPIN, DHTTYPE, 15); // 15 works fine for ESP8266
/*************************** DHT Data   **************************************/
//float humidity, temp_f;  // Values read from sensor Fº
float humidity, temp_c;  // Values read from sensor as Celsius
String webString = "";   // String to display
/*************************** DHT Timing   ************************************/
// Generally, you should use "unsigned long" for variables that hold time
unsigned long previousMillis = 0;        // will store last temp was read
const long interval = 2000;              // interval at which to read sensor

void handle_root() {
  server.send(200, "text/plain", "Hello from the weather esp8266, read from /temp or /humidity");
  delay(100);
}

/****************************** Setup  ***************************************/
void setup(void)
{
  // You can open the Arduino IDE Serial Monitor window to see what the code is doing
  Serial.begin(115200);  // Serial connection from ESP-01 via 3.3v console cable
  dht.begin();           // initialize temperature sensor

  // Connect to WiFi network
  WiFi.begin(ssid, password);
  Serial.print("\n\r \n\rWorking to connect");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("DHT Weather Reading Server");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  server.on("/", handle_root);

  server.on("/temp", []() { // if you add this subdirectory to your webserver call, you get text below :)
    gettemperature();       // read sensor
    //webString="Temperature: "+String((int)temp_f)+" F";   // Arduino has a hard time with float to string
    webString = "Temperature: " + String((int)temp_c) + " C"; // Arduino has a hard time with float to string
    server.send(200, "text/plain", webString);            // send to someones browser when asked
  });

  server.on("/humidity", []() { // if you add this subdirectory to your webserver call, you get text below :)
    gettemperature();           // read sensor
    webString = "Humidity: " + String((int)humidity) + "%";
    server.send(200, "text/plain", webString);               // send to someones browser when asked
  });

  server.begin();
  Serial.println("HTTP server started");
}

/****************************** Loop  ***************************************/
void loop(void)
{
  server.handleClient();
}

/****************************** Get Data From DHTxx  ************************/
void gettemperature() {
  // Wait at least 2 seconds seconds between measurements.
  // if the difference between the current time and last time you read
  // the sensor is bigger than the interval you set, read the sensor
  // Works better than delay for things happening elsewhere also
  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= interval) {
    // save the last time you read the sensor
    previousMillis = currentMillis;

    // Reading temperature for humidity takes about 250 milliseconds!
    // Sensor readings may also be up to 2 seconds 'old' (it's a very slow sensor)
    humidity = dht.readHumidity();          // Read humidity (percent)
    //temp_f = dht.readTemperature(true);     // Read temperature as Fahrenheit
    temp_c = dht.readTemperature();    // Read temperature as Celsius
    // Check if any reads failed and exit early (to try again).
    //if (isnan(humidity) || isnan(temp_f)) {
    if (isnan(humidity) || isnan(temp_c)) {
      Serial.println("Failed to read from DHT sensor!");
      return;
    }
  }
}
