/*
  Project name:
     05 #kidSerie - Adafruit-IO Meets ESP8266, Arduino & MIT App Inventor 2
     (Awesome Smart Phone App!)
     Flavour IV -
     Hex File: _05_kidSerie_sketch_04.Adafruit_Forth_Try.ino
   Revision History:
     20161029:
       - from github adafruit/Adafruit_MQTT_Library
  https://github.com/adafruit/Adafruit_MQTT_Library/blob/master/examples/mqtt_esp8266/mqtt_esp8266.ino
  https://learn.adafruit.com/mqtt-adafruit-io-and-you/intro-to-adafruit-mqtt

   Description:
     Code sequence to get the final flavour V. Follow,please!
     Simple code to intro Adafruit-io technology using MQTT broker, ESP8266, DHTXX sensor & arduino.
     We will publish two topics: temp and humidity and subscribe them also.
     We will access the our dashboard by phone using MIT App Inventor 2.
     Awesome project!!!

   MCU:                  Arduino 1.6.12 - @16MHz       http://www.arduino.cc/
   ESP8266:              ESP-01                        https://goo.gl/E0Eq4l
   DHT11:                PRODUCT ID: 386               https://www.adafruit.com/product/386

   Connections:
      See Official youtube channel vids:
   Based on:   Tony DiCola for Adafruit Industries - see below!

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License v3 as published by
   the Free Software Foundation
*/
/***************************************************
  Adafruit MQTT Library ESP8266 Example
  Must use ESP8266 Arduino from:
    https://github.com/esp8266/Arduino
  Works great with Adafruit's Huzzah ESP board & Feather
  ----> https://www.adafruit.com/product/2471
  ----> https://www.adafruit.com/products/2821
  Adafruit invests time and resources providing this open source code,
  please support Adafruit and open-source hardware by purchasing
  products from Adafruit!
  Written by Tony DiCola for Adafruit Industries.
  MIT license, all text above must be included in any redistribution
 ****************************************************/
/******************************* Adafruit MQTT *******************************/

#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"

/******************************* Esp8266**************************************/
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
/******************************* DHT Config **********************************/
#include <DHT.h>
#define DHTTYPE DHT11
#define DHTPIN  2

/************************* WiFi Access Point *********************************/

#define WLAN_SSID       "colibri" //"...your SSID..."
#define WLAN_PASS       "jr10589238"//"...your password..."

/************************* Adafruit.io Setup *********************************/

#define AIO_SERVER      "io.adafruit.com"
#define AIO_SERVERPORT  1883                   // use 8883 for SSL
#define AIO_USERNAME    "giljr"// "...your AIO username (see https://accounts.adafruit.com)..."
#define AIO_KEY         "6dfe661db07e4b3cbd109d064c778e55" // "...your AIO key..."

/************ Global State (you don't need to change this!) ******************/

// Create an ESP8266 WiFiClient class to connect to the MQTT server.
WiFiClient client;
// or... use WiFiFlientSecure for SSL
//WiFiClientSecure client;

// Setup the MQTT client class by passing in the WiFi client and MQTT server and login details.
Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_KEY);

/****************************** Feeds ***************************************/
/****************************** Topics ***************************************/
//  Setup a feed called 'temp_topic' for publishing.
// Notice MQTT paths for AIO follow the form: <username>/feeds/<feedname>
Adafruit_MQTT_Publish temp_topic = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/temp");
/****************************** Subscriptions*********************************/
// Setup a feed called 'temp_subscription' for subscribing to changes.
Adafruit_MQTT_Subscribe temp_subscription = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/temp");

/*************************** MQTT Conn  ************************************/

// Bug workaround for Arduino 1.6.6, it seems to need a function declaration
// for some reason (only affects ESP8266, likely an arduino-builder bug).
void MQTT_connect();

/*************************** DHT Init   **************************************/
// Initialize DHT sensor
// NOTE: For working with a faster than ATmega328p 16 MHz Arduino chip, like an ESP8266,
// you need to increase the threshold for cycle counts considered a 1 or 0.
// You can do this by passing a 3rd parameter for this threshold.  It's a bit
// of fiddling to find the right value, but in general the faster the CPU the
// higher the value.  The default for a 16mhz AVR is a value of 6.  For an
// Arduino Due that runs at 84mhz a value of 30 works.
// This is for the ESP8266 processor on ESP-01
DHT dht(DHTPIN, DHTTYPE, 11); // 11 works fine for ESP8266
/*************************** DHT Data   **************************************/
//float humidity, temp_f;  // Values read from sensor Fahrenheit
float humidity, temp_c;  // Values read from sensor as Celsius
String tempString = "";   // String to display
/*************************** DHT Timing   ************************************/
// Generally, you should use "unsigned long" for variables that hold time
unsigned long previousMillis = 0;        // will store last temp was read
const long interval = 2000;              // interval at which to read sensor

/****************************** Setup  ***************************************/
void setup() {
  Serial.begin(115200);
  delay(10);

  Serial.println(F("Adafruit MQTT demo"));

  // Connect to WiFi access point.
  Serial.println(); Serial.println();
  Serial.print("Connecting to ");
  Serial.println(WLAN_SSID);

  WiFi.begin(WLAN_SSID, WLAN_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();

  Serial.println("WiFi connected");
  Serial.println("IP address: "); Serial.println(WiFi.localIP());

  // Setup MQTT subscription for onoff feed.
  mqtt.subscribe(&temp_subscription);
}

//uint32_t x = 0;
/****************************** Loop  ***************************************/
void loop() {
  gettemperature() ;
  // Ensure the connection to the MQTT server is alive (this will make the first
  // connection and automatically reconnect when disconnected).  See the MQTT_connect
  // function definition further below.
  MQTT_connect();

  // this is our 'wait for incoming subscription packets' busy subloop
  // try to spend your time here

  /****************************** Subscript  ***********************************/
  Adafruit_MQTT_Subscribe *subscription;
  while ((subscription = mqtt.readSubscription(5000))) {
    if (subscription == &temp_subscription) {
      Serial.print(F("Got: "));
      Serial.println((char *)temp_subscription.lastread);
    }
  }

  /****************************** Publish  **********************************/
  // Now we can publish stuff!
  Serial.print(F("\nSending temp val "));
  Serial.print(temp_c);
  Serial.print("...");
  if (! temp_topic.publish(temp_c)) {
    Serial.println(F("Failed"));
  } else {
    Serial.println(F("OK!"));
  }

  /****************************** Ping  ***************************************/
  // ping the server to keep the mqtt connection alive
  // NOT required if you are publishing once every KEEPALIVE seconds
  /*
    if(! mqtt.ping()) {
    mqtt.disconnect();
    }
  */
}
/********************************* MQTT Conn  ********************************/
// Function to connect and reconnect as necessary to the MQTT server.
// Should be called in the loop function and it will take care if connecting.
void MQTT_connect() {
  int8_t ret;

  // Stop if already connected.
  if (mqtt.connected()) {
    return;
  }

  Serial.print("Connecting to MQTT... ");

  uint8_t retries = 3;
  while ((ret = mqtt.connect()) != 0) { // connect will return 0 for connected
    Serial.println(mqtt.connectErrorString(ret));
    Serial.println("Retrying MQTT connection in 5 seconds...");
    mqtt.disconnect();
    delay(5000);  // wait 5 seconds
    retries--;
    if (retries == 0) {
      // basically die and wait for WDT to reset me
      while (1);
    }
  }
  Serial.println("MQTT Connected!");
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
    //humidity = dht.readHumidity();          // Read humidity (percent)
    //temp_f = dht.readTemperature(true);     // Read temperature as Fahrenheit
    temp_c = dht.readTemperature();    // Read temperature as Celsius
    // Check if any reads failed and exit early (to try again).
    if (isnan(temp_c) || temp_c != 0 )
      if (isnan(temp_c)) {
        Serial.println("Failed to read from DHT sensor!");
        return;
      }
  }
}
