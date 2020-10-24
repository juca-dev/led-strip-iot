#ifndef LED_STRIP_H
#define LED_STRIP_H

#include <Adafruit_NeoPixel.h>

Adafruit_NeoPixel _strip = Adafruit_NeoPixel(9999, D4, NEO_GRB + NEO_KHZ800);

class Device
{
private:
public:
  Device()
  {
  }
  void setup()
  {
    _strip.begin();

    this->clear();
    delay(500);
    Serial.println("Device: ready");
  }
  void update(uint16_t leds, uint16_t pin)
  {
    _strip.setPin(pin);
    _strip.updateLength(leds);
  }
  void clear()
  {
    for (uint16_t i = 0; i < _strip.numPixels(); i++)
    {
      _strip.setPixelColor(i, _strip.Color(0, 0, 0)); //change RGB color value here
    }
    _strip.show();
  }
  void setBrightness(byte value)
  {
    _strip.setBrightness(value);
  }
  void setAll(byte r, byte g, byte b)
  {
    _strip.fill(_strip.Color(r, g, b), 0, _strip.numPixels()); //change RGB color value here
    _strip.show();
  }
  int *split(String value)
  {
    int result[(_strip.numPixels() * 3) + 1];

    char data[value.length() + 1];
    value.toCharArray(data, value.length() + 1);

    int i = 0;
    char delim[] = ",";
    char *ptr = strtok(data, delim);
    while (ptr != NULL)
    {
      result[i++] = atoi(ptr);
      ptr = strtok(NULL, delim);
    }
    return result;
  }
  void setCustom(String data)
  {
    int *res = split(data);
    int length = res[0];
    for (int i = 0; i < length; i++)
    {
      int v = (i * 3) + 1;
      int r = res[v];
      int g = res[v + 1];
      int b = res[v + 2];
      _strip.setPixelColor(i, _strip.Color(r, g, b)); //change RGB color value here
    }
    _strip.show();
  }
};

#endif