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
  // Set LED pins as outputs
  pinMode(R_PIN, OUTPUT);
  pinMode(G_PIN, OUTPUT);
  pinMode(B_PIN, OUTPUT);

  // Create LED srtip object
  // FastLED.addLeds<WS2812, LED_PIN, GRB>(leds, NUM_LEAVES * 6);

  // Start Serial (debugging purposes)
  Serial.begin(115200);

  // Give name and ID to device
  rest.set_id("1");
  rest.set_name("neoleaf_1");

  // Functions exposed to rest api
  rest.function("led", ledControl);
  rest.function("brightness", brightnessControl);
  rest.function("setColour", colourControl);

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

}

void loop() {
  // Handle REST calls
  WiFiClient client = server.available();
  if (!client) {
    return;
  }
  while (!client.available()) {
    delay(1);
  }
  rest.handle(client);
}
