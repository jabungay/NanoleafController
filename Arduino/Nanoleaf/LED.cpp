#include "LED.h"
#include "HelperFunctions.h"
#include <string>

int ledControl(String data) {
  String argList[2];
  seperateData(data, argList, 2);
  
  if (argList[0] == "R") {
      if(argList[1] == "on") {
        digitalWrite(R_PIN, HIGH);
      } else if (argList[1] = "off") {
        digitalWrite(R_PIN, LOW);
      }
  } else if (argList[0] == "G") {
      if(argList[1] == "on") {
        digitalWrite(G_PIN, HIGH);
      } else if (argList[1] = "off") {
        digitalWrite(G_PIN, LOW);
      }
  } else if (argList[0] == "B") {
      if(argList[1] == "on") {
        digitalWrite(B_PIN, HIGH);
      } else if (argList[1] = "off") {
        digitalWrite(B_PIN, LOW);
      }
  }
  return 200;
}

int brightnessControl(String data) {
  analogWrite(R_PIN, data.toInt());
  return 200;
}

int colourControl(String data) {
  String argList[3];
  seperateData(data, argList, 3);

  int red   = map(argList[0].toInt(), 0, 255, 0, 1023);
  int green = map(argList[1].toInt(), 0, 255, 0, 1023);
  int blue  = map(argList[2].toInt(), 0, 255, 0, 1023);

  analogWrite(R_PIN, red);
  analogWrite(G_PIN, green);
  analogWrite(B_PIN, blue);
}
