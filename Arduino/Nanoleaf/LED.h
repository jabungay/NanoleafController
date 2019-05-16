#ifndef LED_H
#define LED_H

#include "HelperFunctions.h"
#include <string>
#include <FastLED.h>

#define LED_PIN 5
#define NUM_LEAVES 8
#define LEDS_PER_LEAF 9



void setupLED();

// Fades between current and desired colour
int setColour(String data);

// Set the global brightness
int setBrightness(String data);

// Toggle all lights
int toggleAll(String data);

int getRed(String data);
int getGreen(String data);
int getBlue(String data);
#endif
