#ifndef LED_STRIP_H
#define LED_STRIP_H

#include <Adafruit_NeoPixel.h>

Adafruit_NeoPixel _strip = Adafruit_NeoPixel(9999, D4, NEO_GRB + NEO_KHZ800);

class Device
{
private:
  uint16_t _leds;

public:
  Device(uint16_t leds, int pin)
  {
    this->_leds = leds;

    // this->_strip = Adafruit_NeoPixel(leds, pin, NEO_GRB + NEO_KHZ800);
  }
  void setup()
  {
    _strip.begin();

    this->clear();
    delay(500);
    Serial.println("Device: ready");
  }
  void clear()
  {
    for (uint16_t i = 0; i < this->_leds; i++)
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
    for (uint16_t i = 0; i < this->_leds; i++)
    {
      _strip.setPixelColor(i, _strip.Color(r, g, b)); //change RGB color value here
    }
    _strip.show();
  }
  int *split(String value)
  {
    int result[(this->_leds * 3) + 1];

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