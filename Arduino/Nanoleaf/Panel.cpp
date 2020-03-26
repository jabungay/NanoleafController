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

// Calculate the point of a bezier curve at time t
void Panel::CubicBezier(float* bP, float* P0, float* P1, float* P2, float* P3, float t)
{
  for(int i = 0; i < 2; i++)
  {
      bP[i] = pow((1 - t), 3) * P0[i] + 3 * pow((1 - t), 2) * t * P1[i] + 3 * (1 - t) * pow(t, 2) * P2[i] + pow(t, 3) * P3[i];
  }
}

void Panel::SetTileHS(int i, byte hue, byte sat, bool save)
{
  _tileArr[i].SetHS(hue, sat, save);
}

void Panel::TurnTileOff(int i, bool save)
{
  _tileArr[i].TurnOff(save);
}
void Panel::DoubleFade(Tile * t1, Tile * t2, CHSV hsv1, CHSV hsv2, bool save)
{

  float steps    = 100;
  float fadeTime = 20;
  float timePerStep = fadeTime / steps;

  byte diffHue1, diffSat1, diffHue2, diffSat2;
  CHSV oldHSV1, oldHSV2;

  if (t1 != NULL)
  {
    oldHSV1 = t1->GetHSV();
    diffHue1 = hsv1.hue - oldHSV1.hue;
    diffSat1 = hsv1.sat - oldHSV1.sat;
  }
  if (t2 != NULL)
  {
    oldHSV2 = t2->GetHSV();
    diffHue2 = hsv2.hue - oldHSV2.hue;
    diffSat2 = hsv2.sat - oldHSV2.sat;
  }

  for (int i = 0; i < steps; i++)
  {
    if (t1 != NULL)
      t1->SetHSInstant(oldHSV1.hue + (i * diffHue1 / steps), oldHSV1.sat + (i * diffSat1 / steps));
    if (t2 != NULL)
      t2->SetHSInstant(oldHSV2.hue + (i * diffHue2 / steps), oldHSV2.sat + (i * diffSat2 / steps));

    FastLED.show();
    FastLED.delay(timePerStep);
  }

  if (save)
  {
    if (t1 != NULL)
      WriteHSV(t1->GetIndex(), hsv1);
    if (t2 != NULL)
      WriteHSV(t2->GetIndex(), hsv2);
  }
}

void Panel::StartPropagate(int index, byte newHue, byte newSat, bool save)
{
  CHSV newHSV = CHSV(newHue, newSat, 255);

  DoubleFade(&_tileArr[index], &_tileArr[index], newHSV, newHSV, false);

  Tile tile = _tileArr[index];

  Tile * prevTile = tile.previousTile;
  Tile * nextTile = tile.nextTile;

  Propagate(prevTile, nextTile , newHSV);

}

void Panel::Propagate(Tile * reverseTile, Tile * forwardTile, CHSV c)
{
  // Keep going until both directions are null
  if(reverseTile != NULL && forwardTile != NULL)
  {
    DoubleFade(reverseTile, forwardTile, c, c, false);
    Tile * nextRev = (reverseTile->previousTile) != NULL ? (reverseTile->previousTile) : NULL;
    Tile * nextFwd = (forwardTile->nextTile) != NULL ? (reverseTile->nextTile) : NULL;

    Propagate(nextRev, nextFwd, c, c, false);
  }
  else
  {
    return;
  }
}
