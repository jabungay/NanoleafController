#ifndef NANOLEAF_H
#define NANOLEAF_H

#include <FastLED.h>
#include <EEPROM.h>

#include "Arduino.h"

class Tile;

class Panel
{
  public:
    Panel();
    Panel(int tiles, int ledsPerTile, CRGB * ledArr);

    bool IsOn();
    void SetState(bool state);

    CHSV ReadHSV(byte addr);
    void WriteHSV(byte addr, CHSV newHSV);

    // Set the hue and saturation of a specific tile
    void SetTileHS(int i, byte hue, byte sat, bool save);

    // Turn a tile off by setting value to 0
    void TurnTileOff(int i, bool saveS);

    // Fade two tiles at the same time
    void DoubleFade(Tile * t1, Tile * t2, CHSV hsv1, CHSV hsv2, bool save);

    // Propagate a colour through the panel, starting at a certain panel
    void StartPropagate(int index, byte newHue, byte newSat, bool save);
    void Propagate(Tile * reverseTile, Tile * forwardTile, CHSV c);

    // Set the brightness of the entire panel
    void SetPanelBrightness(int brightness);

  protected:
    void CubicBezier(float* bP, float* P0, float* P1, float* P2, float* P3, float t);
    CRGB * _ledArr;
    int _ledsPerTile;

  private:
    Tile * _tileArr;
    bool _isOn = true;
    int _numTiles;
};

class Tile : public Panel
{
  public:
    // Meta functions
    Tile();
    void Initialize(Panel * parentPanel, Tile * previous, int index, int ledsPerTile, CRGB * ledArr);
    void SetNext(Tile * next);

    // State-changing functions
    void SetHS(byte newHue, byte newSat, bool save);
    void SetHSInstant(byte hue, byte sat);
    void TurnOff(bool save);

    // State-getting functions
    CHSV GetHSV();
    int GetIndex();

    Tile * previousTile;
    Tile * nextTile;

  private:
    int _index;
};

#endif
