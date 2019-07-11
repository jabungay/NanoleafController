#include "LED.h"

CRGB leds[NUM_LEAVES * LEDS_PER_LEAF];

bool panelOn = true;


// Called in setup(), here so it can access NUM_LEAVES
void setupLED() { FastLED.addLeds<WS2812, LED_PIN, GRB>(leds, NUM_LEAVES * 9); }

int setColour(String data) {
  String argList[5]; // red, green, blue, index, save
  seperateData(data, argList, 4);

  int index = argList[3].toInt();
  int save  = argList[4].toInt();

  float steps       = 100;        // How many steps to make to fade the colours
  float fadeTime    = 50;         // How long (in ms) the fade should take
  float timePerStep = fadeTime / steps;

  // Determine last colours to fade from them
  float red   = leds[index * LEDS_PER_LEAF].r;
  float green = leds[index * LEDS_PER_LEAF].g;
  float blue  = leds[index * LEDS_PER_LEAF].b;

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
  if (save) { writeColour(index, CRGB(newRed, newGreen, newBlue)); }

  // How much to change each colour on each step
  float diffRed   = (newRed   - red)   / steps;
  float diffGreen = (newGreen - green) / steps;
  float diffBlue  = (newBlue  - blue)  / steps;


  for(int i = 0; i < steps; i++) {

    red   += diffRed;
    green += diffGreen;
    blue  += diffBlue;

    for(int j = index * LEDS_PER_LEAF; j < LEDS_PER_LEAF * (index + 1); j++) {
      leds[j] = CRGB(red, green, blue);
    }
    FastLED.show();
    FastLED.delay(timePerStep);
  }

  byte r = newRed;
  byte g = newGreen;
  byte b = newBlue;
}

// Change the brightness of the entire panel
int setBrightness(String brightness) {

  // Brightness is stored at the end of the EEPROM
  int brightnessAddr = EEPROM.length() - 1;
  
  int newBrightness = brightness.toInt();

  FastLED.setBrightness(newBrightness);
  FastLED.show();
  
  // Only write new brightness if it acutally changed
  if (EEPROM.read(brightnessAddr) != newBrightness) {
    EEPROM.write(brightnessAddr, newBrightness);
    EEPROM.commit();
  }

  return 200;
}


int changeBrightness(String amount) {

    // Brightness is stored at the end of the EEPROM
    int brightnessAddr = EEPROM.length() - 1;

    float steps       = 100;        // How many steps to make to fade the brightness
    float fadeTime    = 50;         // How long (in ms) the fade should take
    float timePerStep = fadeTime / steps;

    // Determine the amount to brighten in 256ths
    float change = (amount.toInt() / 100.0) * 256;

    // Retrieve current brightness from EEPROM
    float currentBrightness = EEPROM.read(brightnessAddr);

    float diff = change / steps;
    
    for (int i = 0; i < steps; i++) {
      currentBrightness += diff;

      if (currentBrightness > 255) {
        currentBrightness = 255;
        break;
      } else if (currentBrightness < 5) {
        currentBrightness = 5;
        break;
      }
      
      FastLED.setBrightness(currentBrightness);
      FastLED.show();
      FastLED.delay(timePerStep);
    }
    
    // Only write new brightness if it acutally changed
    if (currentBrightness != EEPROM.read(brightnessAddr)) {
      EEPROM.write(brightnessAddr, currentBrightness);
      EEPROM.commit();
    }

    return 200;
}

int setProfile(String data) {
  // There should be an R, G, and B for each panel
  int length = NUM_LEAVES * 3;

  Serial.println(data);

  String argList[length];
  seperateData(data, argList, length);

  for (int i = 0; i < length / 3; i++) {
    String colour = argList[i * 3] + "," + argList[i * 3 + 1] + "," + argList[i * 3 + 2];
    setColour(colour + "," + String(i) + ",1");
  }
}

int changeState(String data) {
  // If we want to toggle the panel or turn it off, and it's actually on
  if ((data == "toggle" || data == "off") && panelOn) {
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
    // If we want to toggle the panel or turn it on, and it's actually off
  } else if ((data == "toggle" || data == "on") && !panelOn) {
    panelOn = !panelOn;

    for (int i = 0; i < NUM_LEAVES; i++) {

      // Read the colour from EEPROM
      CRGB c = readColour(i);

      // Restore brightness
      FastLED.setBrightness(EEPROM.read(EEPROM.length() - 1));

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

// Send colours to the server
int getRed(String index)   { return readColour(index.toInt()).r; }

int getGreen(String index) { return readColour(index.toInt()).g; }

int getBlue(String index)  { return readColour(index.toInt()).b; }
