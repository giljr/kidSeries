/*
  Project name:
     05 #kidSerie - Adafruit-IO Meets ESP8266, Arduino & MIT App Inventor 2
     (Awesome Smart Phone App!)
     Flavour III -
     Hex File: _06_kidSerie_sketch_02.Adafruit_fake_feeds.ino
   Revision History:
     20161029:
       - from github adafruit/Adafruit_MQTT_Library
  https://github.com/adafruit/Adafruit_MQTT_Library/blob/master/examples/mqtt_esp8266/mqtt_esp8266.ino
  https://learn.adafruit.com/mqtt-adafruit-io-and-you/intro-to-adafruit-mqtt
   Description:
     How to teach young people how to take advantage of already tested codes and create a new one from them.
     I decided to face a new challenge. This code is the second one and joining the first one we will communicate with 
     the Adafruit-io Broker to send temperature and humidity data via MQTT Protocol.
   MCU:                  Arduino 1.6.12 - @16MHz       http://www.arduino.cc/
   ESP8266:              ESP-01                        https://goo.gl/E0Eq4l
   DHT11:                PRODUCT ID: 386               https://www.adafruit.com/product/386
   Connections:
      See Official youtube channel vids: https://youtu.be/cwYix_Oyui4
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

/************************* WiFi Access Point *********************************/

#define WLAN_SSID       "...your SSID..."
#define WLAN_PASS       "...your password..."

/************************* Adafruit.io Setup *********************************/

#define AIO_SERVER      "io.adafruit.com"
#define AIO_SERVERPORT  1883                   // use 8883 for SSL
#define AIO_USERNAME    "...your AIO username (see https://accounts.adafruit.com)..."
#define AIO_KEY         "...your AIO key..."

/************ Global State (you don't need to change this!) ******************/

// Create an ESP8266 WiFiClient class to connect to the MQTT server.
WiFiClient client;
// or... use WiFiFlientSecure for SSL
//WiFiClientSecure client;

// Setup the MQTT client class by passing in the WiFi client and MQTT server and login details.
Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_KEY);

/****************************** Feeds ***************************************/
/****************************** Topics ***************************************/
// Setup a feed called 'photocell' for publishing.
// Notice MQTT paths for AIO follow the form: <username>/feeds/<feedname>
Adafruit_MQTT_Publish photocell = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/photocell");
/****************************** Subscriptions*********************************/
// Setup a feed called 'photocell' for subscribing to changes.
Adafruit_MQTT_Subscribe photocell_subsc = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/photocell");

/***************************  MQTT Conn   ************************************/

// Bug workaround for Arduino 1.6.6, it seems to need a function declaration
// for some reason (only affects ESP8266, likely an arduino-builder bug).
void MQTT_connect();

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
  mqtt.subscribe(&photocell_subsc);
}

uint32_t x=0;    // Mock data

/****************************** Loop  ***************************************/
void loop() {
  // Ensure the connection to the MQTT server is alive (this will make the first
  // connection and automatically reconnect when disconnected).  See the MQTT_connect
  // function definition further below.
  MQTT_connect();

  // this is our 'wait for incoming subscription packets' busy subloop
  // try to spend your time here
  
 /****************************** Subscript  ***********************************/
 Adafruit_MQTT_Subscribe *subscription;
  while ((subscription = mqtt.readSubscription(5000))) {
    if (subscription == &photocell_subsc) {
      Serial.print(F("Got: "));
      Serial.println((char *)photocell_subsc.lastread);
    }
  }
 /****************************** Publish  **********************************/
   // Now we can publish stuff!
  Serial.print(F("\nSending photocell val "));
  Serial.print(x);
  Serial.print("...");
  if (! photocell.publish(x++)) {
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
