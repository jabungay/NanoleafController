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

    void SetTileHS(int i, byte hue, byte sat, bool save);
    void TurnTileOff(int i, bool saveS);
    void DoubleFade(Tile * forwardTile, Tile * backwardTile, byte newHue, byte newSat, bool save);

    void SetPanelBrightness(int brightness);

  protected:
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

  private:
    void CubicBezier(float* bP, float* P0, float* P1, float* P2, float* P3, float t);
    Tile * _previousTile;
    Tile * _nextTile;
    int _index;
};

#endif
