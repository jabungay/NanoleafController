// Import required libraries
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <aREST.h>

#include "Nanoleaf.h"
#include "RestFunctions.h"


// Create aREST instance
aREST rest = aREST();

// Unique device id
char* device_id = "F7q1KK";

// WiFi parameters
const char* ssid = "cooze";
const char* password = "20Fathership17";

// Create an instance of the server
WiFiServer server(80);

// Const values for panel properties
const int numTiles    = 8;
const int ledsPerTile = 9;
const int ledPin      = 5;

// Other panel values
CRGB leds[numTiles * ledsPerTile];
Panel p;

/*********************************************************************
 * Functions specifically used to interface with the REST API ********
 ********************************************************************/

// Seperate a comma-seperated string (required for REST) into an array of strings
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

// Send HSV coded as a single int (1 (hue) (sat) (val)
int REST_GetHSV(String index)
{
 CHSV c = p.ReadHSV(index.toInt());

 String hsv[3] = {String(c.h), String(c.s), String(c.v)};

 for (int i = 0; i < 3; i++)
 {
   if (hsv[i].length() == 2)
   {
     hsv[i] = "0" + hsv[i];
   }
   else if (hsv[i].length() == 1)
   {
     hsv[i] = "00" + hsv[i];
   }
 }

 String o = "1" + hsv[0] + hsv[1] + hsv[2];

 return o.toInt();

}

// Set the colour of a given panel (HSV)
int REST_SetColour(String data)
{
   String argList[4]; // hue, sat, index, save
   seperateData(data, argList, 4);

   byte hue  = argList[0].toInt();
   byte sat  = argList[1].toInt();
   int index = argList[2].toInt();
   int save  = argList[3].toInt();

   p.SetTileHS(index, hue, sat, save);

   return 200;
}

// Set the brightness of the entire panel
int REST_SetBrightness(String data)
{
   // Brightness is stored at the end of the EEPROM
   int brightnessAddr = EEPROM.length() - 1;

   int newBrightness = data.toInt();

   FastLED.setBrightness(newBrightness);
   FastLED.show();

   // Only write new brightness if it acutally changed
   if (EEPROM.read(brightnessAddr) != newBrightness) {
     EEPROM.write(brightnessAddr, newBrightness);
     EEPROM.commit();
   }

   return 200;
}

// Turn the panel on and off
int REST_ChangePanelState(String data)
{
   // If we want to toggle the panel or turn it off, and it's actually on
   if ((data == "toggle" || data == "off") && p.IsOn()) {
     p.SetState(!p.IsOn());

     // When turning off, do it backwards
     for (int i = (numTiles - 1); i > -1; i--) {

       // Read the colour from EEPROM
       CHSV c = p.ReadHSV(i);
       bool isOn = c.v;

       // Only turn off panels that aren't already off
       if(isOn) {
         p.TurnTileOff(i, false);
       }
     }
     // If we want to toggle the panel or turn it on, and it's actually off
   } else if ((data == "toggle" || data == "on") && !p.IsOn()) {
     p.SetState(!p.IsOn());

     for (int i = 0; i < numTiles; i++) {

       // Read the colour from EEPROM
       CHSV c = p.ReadHSV(i);

       // Restore brightness
       FastLED.setBrightness(EEPROM.read(EEPROM.length() - 1));

       byte hue = c.h;
       byte sat = c.s;

       bool isOn = c.v;

       // Only try to turn a tile on if it needs to be turned on
       if (isOn) {
         p.SetTileHS(i, hue, sat, false);
       }
     }
   }

   return 200;

}

// Turn off a specific tile
int REST_TurnOff(String data)
{
  String argList[2];  // index, save
  seperateData(data, argList, 2);

  int index = argList[0].toInt();
  bool save = argList[1].toInt();

  p.TurnTileOff(index, save);

  return 200;
}


void setup(void)
{
  // Start Serial (debugging purposes)
  Serial.begin(115200);

  // Give name and ID to device
  rest.set_id(device_id);
  rest.set_name("nanoleaf_1");

  // Connect to WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  // Start the server
  server.begin();
  // Print the IP address
  Serial.println(WiFi.localIP());

  // Create REST functions
  rest.function("getHSV"        , REST_GetHSV           );
  rest.function("setColour"     , REST_SetColour        );
  rest.function("setBrightness" , REST_SetBrightness    );
  rest.function("changeState"   , REST_ChangePanelState );
  rest.function("turnOff"       , REST_TurnOff          );

  // Initialize panel object
  FastLED.addLeds<WS2812, ledPin, GRB>(leds, numTiles * ledsPerTile);
  p = Panel(numTiles, ledsPerTile, leds);

}

void loop() {
  // Handle REST calls
  WiFiClient clientLocal = server.available();
  if (!clientLocal) {
    return;
  }
  while (!clientLocal.available()) {
    FastLED.delay(1);
  }
  rest.handle(clientLocal);
}
