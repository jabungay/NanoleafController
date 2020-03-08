#ifndef RESTFUNCTIONS_H
#define RESTFUNCTIONS_H

void seperateData(String data, String* arr, int arrLen);

int REST_GetHSV(String index);
int REST_SetColour(String data);
int REST_SetBrightness(String data);
int REST_ChangePanelState(String data);
int REST_TurnOff(String data);

#endif
