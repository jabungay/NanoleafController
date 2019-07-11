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


// Change brightness by an absolute percent
// +ve: brighten, -ve: dim
int changeBrightness(String amount);

// Change all panels based on a certain
// profile (stored on the server)
int setProfile(String data);

// Change the state of the entire panel
// @param data: "on", "off", or "toggle"
int changeState(String data);

int getRed(String index);
int getGreen(String index);
int getBlue(String index);
#endif
