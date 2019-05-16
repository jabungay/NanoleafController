#include "LED.h"

CRGB leds[NUM_LEAVES * LEDS_PER_LEAF];
CRGB lastState[NUM_LEAVES];

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
    for (int i = 0; i < NUM_LEAVES; i++) {
      lastState[i] = CRGB(0, 0, 0);
    }
  }

  // Only save the previous values if requested
  // (used to restore color after toggleAll())
  if (save) {
    lastState[index] = CRGB(newRed, newGreen, newBlue);
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

      bool isOn = lastState[i].r || lastState[i].g || lastState[i].b;

      // Only turn off panels that aren't already off
      if(isOn) {
        setColour("0,0,0," + String(i) + ",0");
      }
    }
  } else {
    panelOn = !panelOn;

    for (int i = 0; i < NUM_LEAVES; i++) {

      float red   = lastState[i].r;
      float green = lastState[i].g;
      float blue  = lastState[i].b;

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

  return lastState[index].r;
}

int getGreen(String data) {
  int index = data.toInt();

  return lastState[index].g;
}

int getBlue(String data) {
  int index = data.toInt();

  return lastState[index].b;
}
