#include "device.h"

void Device::setup()
{
  strip.updateType(NEO_GRB + NEO_KHZ800);
  strip.begin();

  this->clear();
  delay(500);
  Serial.println("Device: ready");
}
void Device::update(uint16_t leds, uint16_t pin)
{
  strip.setPin(pin);
  strip.updateLength(leds);
}
void Device::clear()
{
  for (uint16_t i = 0; i < strip.numPixels(); i++)
  {
    strip.setPixelColor(i, strip.Color(0, 0, 0)); //change RGB color value here
  }
  strip.show();
}
void Device::setBrightness(byte value)
{
  strip.setBrightness(value);
}
void Device::setAll(byte r, byte g, byte b)
{
  strip.fill(strip.Color(r, g, b), 0, strip.numPixels()); //change RGB color value here
  strip.show();
}
int* Device::split(String value)
{
  int result[(strip.numPixels() * 3) + 1];

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
void Device::setCustom(String data)
{
  int *res = split(data);
  int length = res[0];
  for (int i = 0; i < length; i++)
  {
    int v = (i * 3) + 1;
    int r = res[v];
    int g = res[v + 1];
    int b = res[v + 2];
    strip.setPixelColor(i, strip.Color(r, g, b)); //change RGB color value here
  }
  strip.show();
}