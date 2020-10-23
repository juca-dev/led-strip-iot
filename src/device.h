#ifndef LED_STRIP_H
#define LED_STRIP_H

#include <Adafruit_NeoPixel.h>

class Device
{
private:
  int _leds;
  Adafruit_NeoPixel _core;

public:
  Device(int leds, int pin)
  {
    this->_leds = leds;

    this->_core = Adafruit_NeoPixel(leds, pin, NEO_GRB + NEO_KHZ800);
    this->_core.begin();

    this->clear();
    delay(500);
    Serial.println("Device: ready");
  }
  void clear()
  {
    for (int i = 0; i < this->_leds; i++)
    {
      this->_core.setPixelColor(i, this->_core.Color(0, 0, 0)); //change RGB color value here
    }
    this->_core.show();
  }
  void setBrightness(byte value)
  {
    this->_core.setBrightness(value);
  }
  void setAll(byte r, byte g, byte b)
  {
    for (int i = 0; i < this->_leds; i++)
    {
      this->_core.setPixelColor(i, this->_core.Color(r, g, b)); //change RGB color value here
    }
    this->_core.show();
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
      this->_core.setPixelColor(i, this->_core.Color(r, g, b)); //change RGB color value here
    }
    this->_core.show();
  }
};

#endif