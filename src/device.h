#ifndef LED_STRIP_H
#define LED_STRIP_H

#include <Adafruit_NeoPixel.h>

class Device
{
private:
  Adafruit_NeoPixel strip;

public:
  void setup();
  StaticJsonDocument<256> config();
  void update(uint16_t leds, uint16_t pin);
  void clear();
  void setBrightness(byte value);
  void setAll(byte r, byte g, byte b);
  int* split(String value);
  void setCustom(String data);
};

#endif