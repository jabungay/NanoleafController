/************************
 * Pin Assignment Legend
 * D0 - 16
 * D1 - 5
 * D2 - 4
 * D3 - 0
 * D4 - 2
 * D5 - 14
 * D6 - 12
 * D7 - 13
 * D8 - 15
 ************************/

// Import required libraries
#include "ESP8266WiFi.h"
#include <aREST.h>
#include "HelperFunctions.h"
#include "LED.h"

// Create aREST instance
aREST rest = aREST();


// WiFi parameters
const char* ssid = "cooze";
const char* password = "20Fathership17";

// Create an instance of the server
WiFiServer server(80);

void setup(void)
{

  // Create emulated EEPROM, 3 bytes for each leaf plus 1 for brightness
  EEPROM.begin(NUM_LEAVES > 1 ? 3 * NUM_LEAVES + 1: 4);

  setupLED();

  // Start Serial (debugging purposes)
  Serial.begin(115200);

  // Give name and ID to device
  rest.set_id("1");
  rest.set_name("nanoleaf_1");

  // Functions exposed to rest api
  rest.function("setColour"    , setColour    );
  rest.function("setBrightness", setBrightness);
  rest.function("setProfile"   , setProfile   );
  rest.function("toggleAll"    , toggleAll    );
  rest.function("getRed"       , getRed       );
  rest.function("getGreen"     , getGreen     );
  rest.function("getBlue"      , getBlue      );
  rest.function("ping"         , ping         );

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

  // Restore colours and brighness on startup
  setBrightness(String(EEPROM.read(EEPROM.length() - 1)));

  for (int i = 0; i < NUM_LEAVES; i++) {
    int r = EEPROM.read(i * 3);
    int g = EEPROM.read(i * 3 + 1);
    int b = EEPROM.read(i * 3 + 2);

    String c = String(r) + "," + String(g) + "," + String(b);
    setColour(c + "," + String(i) + ",0");
  }

}

void loop() {
  // Handle REST calls
  WiFiClient client = server.available();
  if (!client) {
    return;
  }
  while (!client.available()) {
    FastLED.delay(1);
  }
  rest.handle(client);
}
