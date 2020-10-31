#include "device.h"

void Device::setup()
{
  strip.updateType(NEO_GRB + NEO_KHZ800);
  strip.begin();

  this->load(this->config());

  this->clear();
  delay(500);
  Serial.println("Device: ready");
}
StaticJsonDocument<256> Device::config()
{
  String data = this->storage.get("device.json");
  StaticJsonDocument<256> json;
  DeserializationError err = deserializeJson(json, data);
  if (err)
  {
    Serial.print("### ERR: DEVICE - ");
    Serial.println(err.c_str());
  }
  return json;
}
void Device::update(uint16_t leds, byte pin)
{
  strip.setPin(pin);
  strip.updateLength(leds);

  StaticJsonDocument<256> json;
  json["leds"] = leds;
  json["pin"] = pin;
  String value;
  serializeJson(json, value);
  this->storage.put("device.json", value);
}
bool Device::load(StaticJsonDocument<256> json)
{
  if (json.isNull() || !json.containsKey("leds") || !json.containsKey("pin"))
  {
    Serial.println("device: No config");
    return false;
  }

  this->update(json["leds"], json["pin"]);
  return true;
}
void Device::clear()
{
  strip.clear();
  strip.show();
}
void Device::setBrightness(byte value)
{
  strip.setBrightness(value);
  strip.show();
}
void Device::setAll(byte r, byte g, byte b)
{
  strip.fill(strip.Color(r, g, b), 0, strip.numPixels()); //change RGB color value here
  strip.show();
}
int *Device::split(String value)
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