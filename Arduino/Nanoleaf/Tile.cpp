#include "Nanoleaf.h"

// Class constructor
Tile::Tile() : previousTile(NULL), nextTile(NULL) { }

/************************
* BEGIN META FUNCTIONS **
*************************/

// Initialize the tile
void Tile::Initialize(Panel * parentPanel, Tile * previous, int index, int ledsPerTile, CRGB * ledArr)
{
  // If this is not the first tile, set the previous tile's next pointer to this tile
  if (previous)
  {
    previous->SetNext(this);
  }
  _index = index;
  previousTile = previous;
  _ledArr = ledArr;
  _ledsPerTile = ledsPerTile;

  CHSV lastColour = ReadHSV(_index);
  SetHS(lastColour.hue, lastColour.sat, false);

}

// Set the next pointer of the tile
void Tile::SetNext(Tile * t)
{
  nextTile = t;
}

// Get the panel's current colour
CHSV Tile::GetHSV()
{
  return ReadHSV(_index);
}

int Tile::GetIndex() { return _index; }

/**********************
* END META FUNCTIONS **
***********************/


/*************************
* BEGIN LIGHT FUNCTIONS **
**************************/

// Fade from current colour to new colour (HSV with V=255)
void Tile::SetHS(byte newHue, byte newSat, bool save)
{
  int steps         = 100;          // How many steps to make to fade the colours
  float fadeTime    = 50.0;         // How long (in ms) the fade should take
  float timePerStep = fadeTime / steps;

  // Points for Bezier
  float P0[] = {0, 0};
  float P1[] = {0.5, 0.1};
  float P2[] = { -0.5, 1};
  float P3[] = {1, 1};

  CHSV oldHSV = ReadHSV(_index);

  byte hue = oldHSV.h;
  byte sat = oldHSV.s;

  // Since the val may not saved, make sure the panel is on
  bool isOn = _ledArr[_index * _ledsPerTile].r || _ledArr[_index * _ledsPerTile].g || _ledArr[_index * _ledsPerTile].b;
  byte val = isOn ? oldHSV.v : 0;

  byte newVal = 255;

  if (save) {
    WriteHSV(_index, CHSV(newHue, newSat, 255));
  }

  byte diffHue = newHue - hue;
  byte diffSat = newSat - sat;
  byte diffVal = newVal - val;

  for (int i = 0; i < steps; i++)
  {

    // Generate current Bezier location
    float point[2];
    CubicBezier(point, P0, P1, P2, P3, timePerStep * i / fadeTime);

    float hueStep = hue + diffHue * point[1];
    float satStep = sat + diffSat * point[1];
    float valStep = val + diffVal * point[1];

    for (int j = _index * _ledsPerTile; j < _ledsPerTile * (_index + 1); j++)
    {
      _ledArr[j] = CHSV(hueStep, satStep, valStep);
    }

    FastLED.show();
    FastLED.delay(timePerStep);
  }
}

// Set the tile's colour instantly
void Tile::SetHSInstant(byte hue, byte sat)
{
  for (int i = _index; i < _ledsPerTile * (_index + 1); i++)
  {
    _ledArr[i] = CHSV(hue, sat, 255);
  }
}

// Turn the tile off by setting val to 0
void Tile::TurnOff(bool save)
{
   int steps         = 100;          // How many steps to make to fade the colours
   float fadeTime    = 50.0;         // How long (in ms) the fade should take
   float timePerStep = fadeTime / steps;

   // Points for Bezier
   float P0[] = {0    , 0   };
   float P1[] = {0.5  , 0.1 };
   float P2[] = {-0.5 , 1   };
   float P3[] = {1    , 1   };

   CHSV currentHSV = ReadHSV(_index);
   Serial.println(currentHSV.v);
   Serial.println();

   for(int i = 0; i < steps; i++)
   {
     // Generate current Bezier location
     float point[2];
     CubicBezier(point, P0, P1, P2, P3, timePerStep * i / fadeTime);
     float valStep = currentHSV.v - currentHSV.v * point[1];

     for(int j = _index * _ledsPerTile; j < _ledsPerTile * (_index + 1); j++) {
       _ledArr[j] = CHSV(currentHSV.h, currentHSV.s, valStep);
     }

     FastLED.show();
     FastLED.delay(timePerStep);
   }

   if (save)
     WriteHSV(_index, CHSV(currentHSV.h, currentHSV.s, 0));

}

/**********************
* END LIGHT FUNCTIONS **
***********************/
