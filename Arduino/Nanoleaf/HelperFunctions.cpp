#include "HelperFunctions.h"

void seperateData(String data, String* arr, int arrLen) {
  int arguments = 0;
  int lastSep = -1;

  // Iterate through the data and add arguments to the list
  for(int i = 0; i < data.length(); i++) {
    if(data.charAt(i) == ',') {
      arr[arguments] = data.substring(lastSep + 1, i);
      lastSep = i;
      arguments++;
    }
  }
  // Add the final argument
  arr[arguments] = data.substring(lastSep + 1, data.length());

}

int ping(String data) {
  return 200;
}

CRGB readColour(int addr) {
  byte r = EEPROM.read(addr * 3    );
  byte g = EEPROM.read(addr * 3 + 1);
  byte b = EEPROM.read(addr * 3 + 2);

  return CRGB(r,g,b);
}

void writeColour(int addr, CRGB newColour) {

  // To minmize write cycles, only write when the value actually changes
  CRGB currentColour = readColour(addr);

  if (currentColour.r != newColour.r) {
    byte r = newColour.r;
    EEPROM.write(addr * 3, r);
  }
  if (currentColour.g != newColour.g) {
    byte g = newColour.g;
    EEPROM.write(addr * 3 + 1, g);
  }
  if (currentColour.b != newColour.b) {
    byte b = newColour.b;
    EEPROM.write(addr * 3 + 2, b);
  }
  EEPROM.commit();
}

void clearColours() {
  // Don't want to clear the brightness
  for (int i = 0; i < EEPROM.length() - 1; i++) {
    EEPROM.write(i, 0);
  }
  EEPROM.commit();
}
