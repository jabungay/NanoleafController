#include "LED.h"

CRGB leds[NUM_LEAVES * LEDS_PER_LEAF];

bool panelOn = true;

void setupLED() {
  FastLED.addLeds<WS2812, LED_PIN, GRB>(leds, NUM_LEAVES * 9);
}

int setColour(String data) {
  String argList[5]; // red, green, blue, index, save
  seperateData(data, argList, 4);

  int index = argList[3].toInt();
  int save  = argList[4].toInt();


  float steps = 100;       // How many steps to make to fade the colours
  float fadeTime = 50;  // How long (in ms) the fade should take
  float timePerStep = fadeTime / steps;

  // Determine last colours to fade from them
  float oldRed   = leds[index * LEDS_PER_LEAF].r;
  float oldGreen = leds[index * LEDS_PER_LEAF].g;
  float oldBlue  = leds[index * LEDS_PER_LEAF].b;

  // New colours
  float newRed   = argList[0].toInt();
  float newGreen = argList[1].toInt();
  float newBlue  = argList[2].toInt();


  // If we get here and the panel is set as off, set it as on before continuing
  if (save && !panelOn) {
    panelOn = true;
    clearColours();
  }

  // Only save the previous values if requested
  // (used to restore color after toggleAll())
  if (save) {
    writeColour(index, CRGB(newRed, newGreen, newBlue));
  }

  // How much to change each colour on each step
  float diffRed   = (newRed   - oldRed)   / steps;
  float diffGreen = (newGreen - oldGreen) / steps;
  float diffBlue  = (newBlue  - oldBlue)  / steps;

  for(int i = 0; i < steps; i++) {

    oldRed += diffRed;
    oldGreen += diffGreen;
    oldBlue += diffBlue;

    for(int j = index * LEDS_PER_LEAF; j < LEDS_PER_LEAF * (index + 1); j++) {
      leds[j] = CRGB(oldRed, oldGreen, oldBlue);
    }
    FastLED.show();
    FastLED.delay(timePerStep);
  }

  byte r = newRed;
  byte g = newGreen;
  byte b = newBlue;
}

int setBrightness(String data) {
  int newBrightness = data.toInt();
  Serial.println(newBrightness);
  FastLED.setBrightness(newBrightness);
}

int toggleAll(String data) {
  if (panelOn) {
    panelOn = !panelOn;

    // When turning off, do it backwards
    for (int i = (NUM_LEAVES - 1); i > -1; i--) {

      // Read the colour from EEPROM
      CRGB c = readColour(i);
      bool isOn = c.r || c.g || c.b;

      // Only turn off panels that aren't already off
      if(isOn) {
        setColour("0,0,0," + String(i) + ",0");
      }
    }
  } else {
    panelOn = !panelOn;

    for (int i = 0; i < NUM_LEAVES; i++) {

      // Read the colour from EEPROM
      CRGB c = readColour(i);
      
      float red   = c.r;
      float green = c.g;
      float blue  = c.b;

      bool isOn = red || green || blue;

      String colour = String(red) + "," + String(green) + "," + String(blue) + "," + String(i) + ",0";

      // Only try to turn a panel on if it needs to be turned on
      if (isOn) {
        setColour(colour);
      }
    }
  }

  return 200;
}

int getRed(String data) {
  int index = data.toInt();

  return readColour(index).r;
}

int getGreen(String data) {
  int index = data.toInt();

  return readColour(index).g;
}

int getBlue(String data) {
  int index = data.toInt();

  return readColour(index).b;
}
