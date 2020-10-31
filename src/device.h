#ifndef LED_STRIP_H
#define LED_STRIP_H

#include <Adafruit_NeoPixel.h>
#include <ArduinoJson.h>

#include "storage.h"

class Device
{
private:
  Adafruit_NeoPixel strip;
  Storage storage;

public:
  void setup();
  StaticJsonDocument<256> config();
  void update(uint16_t leds, byte pin);
  bool load(StaticJsonDocument<256> json);
  void clear();
  void setBrightness(byte value);
  void setAll(byte r, byte g, byte b);
  int* split(String value);
  void setCustom(String data);
};

#endif