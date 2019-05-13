#include <Arduino.h>

#ifndef HELPERS_H
#define HELPERS_H

// Seperates comma-delimited data given the data, an array to input to,
// and the expected number of variables
void seperateData(String data, String* arr, int arrLen);

// Function to ping the device, ensuring it's available
int ping(String data);

#endif
