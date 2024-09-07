// Adafruit IO Digital Output Example
// Tutorial Link: https://learn.adafruit.com/adafruit-io-basics-digital-output
//
// Adafruit invests time and resources providing this open source code.
// Please support Adafruit and open source hardware by purchasing
// products from Adafruit!
//
// Written by Todd Treece for Adafruit Industries
// Copyright (c) 2016 Adafruit Industries
// Licensed under the MIT license.
//
// All text above must be included in any redistribution.

/************************** Configuration ***********************************/

// Edit the config.h tab and enter your Adafruit IO credentials
// and any additional configuration needed for WiFi, cellular,
// or ethernet clients.
#include "config.h"

/************************ Example Starts Here *******************************/

// Digital pin definitions
#define LED_PIN_1 D0
#define LED_PIN_2 D5

// Set up the 'digital' feeds
AdafruitIO_Feed *led1Feed = io.feed("LED1");
AdafruitIO_Feed *led2Feed = io.feed("LED2");

void setup() {
  
  pinMode(LED_PIN_1, OUTPUT);
  pinMode(LED_PIN_2, OUTPUT);
  
  // Start the serial connection
  Serial.begin(9600);

  // Wait for serial monitor to open
  while (!Serial);

  // Connect to io.adafruit.com
  Serial.print("Connecting to Adafruit IO");
  io.connect();

  // Set up message handlers for the feeds
  led1Feed->onMessage(handleMessage1);
  led2Feed->onMessage(handleMessage2);

  // Wait for connection
  while (io.status() < AIO_CONNECTED) {
    Serial.print(".");
    delay(500);
  }

  // Connection established
  Serial.println();
  Serial.println(io.statusText());
  led1Feed->get();
  led2Feed->get();
}

void loop() {

  // io.run() is required for all sketches.
  // It keeps the client connected to io.adafruit.com and processes incoming data.
  io.run();
}

// This function is called whenever a message is received from the 'LED_Blinking' feed.
void handleMessage1(AdafruitIO_Data *data) {

  Serial.print("received <- ");

  if (data->toPinLevel() == HIGH)
    Serial.println("LED1 HIGH");
  else
    Serial.println("LED1 LOW");

  digitalWrite(LED_PIN_1, data->toPinLevel());
}

// This function is called whenever a message is received from the 'LED2' feed.
void handleMessage2(AdafruitIO_Data *data) {

  Serial.print("received <- ");

  if (data->toPinLevel() == HIGH)
    Serial.println("LED2 HIGH");
  else
    Serial.println("LED2 LOW");

  digitalWrite(LED_PIN_2, data->toPinLevel());
}
