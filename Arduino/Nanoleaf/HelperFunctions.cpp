#include "HelperFunctions.h"

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

  // DEBUG: print all arguments
  for(int i = 0; i < arrLen; i++) {
    Serial.println(arr[i]);  
  }
  
}
