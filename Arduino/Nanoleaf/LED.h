#ifndef LED_H
#define LED_H

#include "HelperFunctions.h"


#define LED_PIN 5
#define NUM_LEAVES 8
#define LEDS_PER_LEAF 9


void setupLED();

// Fades between current and desired colour
int setColour(String data);

// Set the global brightness
int setBrightness(String brightness);

// Change all panels based on a certain
// profile (stored on the server)
int setProfile(String data);

// Toggle all lights
int toggleAll(String data);

int getRed(String index);
int getGreen(String index);
int getBlue(String index);
#endif
