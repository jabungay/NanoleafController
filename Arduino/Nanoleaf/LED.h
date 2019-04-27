//#include <FastLED.h>

#include <Arduino.h>

#ifndef LED
#define LED

// Single LED definitions (Testing Purposes)
#define R_PIN 14
#define G_PIN 12
#define B_PIN 13

//// Definition for WS2812B
//#define LED_PIN 13
//#define NUM_LEAVES 9  // 6 LEDs per leaf
//
//CRGB leds[NUM_LEAVES * 6]; // 6 LEDs per leaf

int ledControl (String data);

int brightnessControl(String data);

// Takes an RGB colour code and changes the LED to it
int colourControl(String data);

#endif
