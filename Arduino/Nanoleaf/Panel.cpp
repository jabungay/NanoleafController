#include "Nanoleaf.h"

// Class constructors
Panel::Panel() { }

Panel::Panel(int tiles, int ledsPerTile, CRGB * ledArr) : _ledArr(ledArr), _tileArr(new Tile[tiles]), _numTiles(tiles), _ledsPerTile(ledsPerTile)
{
  // Init. EEPROM
  EEPROM.begin(3 * tiles + 1);

  FastLED.setBrightness(EEPROM.read(EEPROM.length() - 1));

  for (int i = 0; i < tiles; i++)
  {
    _tileArr[i].Initialize(this, i > 1 ? &_tileArr[i - 1] : NULL, i, _ledsPerTile, _ledArr);
  }
}

bool Panel::IsOn() { return _isOn; }
void Panel::SetState(bool state) { _isOn = state; }

CHSV Panel::ReadHSV(byte addr)
{
  byte h = EEPROM.read(addr * 3);
  byte s = EEPROM.read(addr * 3 + 1);
  byte v = EEPROM.read(addr * 3 + 2);

  return CHSV(h,s,v);
}

void Panel::WriteHSV(byte addr, CHSV newHSV)
{
  CHSV currentHSV = ReadHSV(addr);

  if (currentHSV.h != newHSV.h)
  {
    EEPROM.write(addr * 3, newHSV.h);
  }
  if (currentHSV.s != newHSV.s)
  {
    EEPROM.write(addr * 3 + 1, newHSV.s);
  }
  if (currentHSV.v != newHSV.v)
  {
    EEPROM.write(addr * 3 + 2, newHSV.v);
  }
  EEPROM.commit();
}

void Panel::SetTileHS(int i, byte hue, byte sat, bool save)
{
  _tileArr[i].SetHS(hue, sat, save);
}

void Panel::TurnTileOff(int i, bool save)
{
  _tileArr[i].TurnOff(save);
}

// Fade two tiles at the same time
void Panel::DoubleFade(Tile * forwardTile, Tile * backwardTile, byte newHue, byte newSat, bool save)
{

}
