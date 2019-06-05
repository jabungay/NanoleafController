#ifndef HELPERS_H
#define HELPERS_H

#include <Arduino.h>
#include <FastLED.h>
#include <EEPROM.h>
#include <string>

// Seperates comma-delimited data given the data, an array to input to,
// and the expected number of variables
void seperateData(String data, String* arr, int arrLen);

// Function to ping the device, ensuring it's available
int ping(String data);

// Reads a colour stored in EEPROM at addr, where each addr represents 3 bytes
CRGB readColour(int addr);

// Set the colour at addr in EEPROM, but only overwrite values that actually changed
void writeColour(int addr, CRGB newColour);

// Erase all colour data in the EEPROM
void clearColours();

#endif
