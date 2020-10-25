#include "api.h"

StaticJsonDocument<256> Api::getJson()
{
  String data = server.arg("plain");
  Serial.print("API: data ");
  Serial.println(data);
  StaticJsonDocument<256> json;
  DeserializationError err = deserializeJson(json, data);
  if (err)
  {
    Serial.print("### ERR: API - ");
    Serial.println(err.c_str());
  }
  return json;
}
StaticJsonDocument<256> Api::getRGB()
{
  String data = this->storage.get("rgb.json");
  StaticJsonDocument<256> json;
  DeserializationError err = deserializeJson(json, data);
  if (err)
  {
    Serial.print("### ERR: API - ");
    Serial.println(err.c_str());
  }
  return json;
}
StaticJsonDocument<256> Api::getDevice()
{
  String data = this->storage.get("device.json");
  StaticJsonDocument<256> json;
  DeserializationError err = deserializeJson(json, data);
  if (err)
  {
    Serial.print("### ERR: API - ");
    Serial.println(err.c_str());
  }
  return json;
}

StaticJsonDocument<256> Api::getWifi()
{
  String data = this->storage.get("wifi.json");
  StaticJsonDocument<256> json;
  DeserializationError err = deserializeJson(json, data);
  if (err)
  {
    Serial.print("### ERR: API - ");
    Serial.println(err.c_str());
  }
  return json;
}

Api::Api(byte pinLed)
{
  this->ledPin = pinLed;
  Serial.println("API: ready");
}

void Api::setup()
{
  pinMode(this->ledPin, OUTPUT);

  this->storage.setup();

  this->device.setup();
  StaticJsonDocument<256> json = this->getDevice();
  this->loadDevice(json);
  json = this->getRGB();
  this->loadRGB(json);

  this->server.on("/", HTTP_GET, std::bind(&Api::conWeb, this));
  this->server.on("/toggle", HTTP_POST, std::bind(&Api::conToggle, this));
  this->server.on("/wifi", HTTP_POST, std::bind(&Api::conSetWifi, this));
  this->server.on("/wifi", HTTP_GET, std::bind(&Api::conWifi, this));
  this->server.on("/rgb", HTTP_POST, std::bind(&Api::conSetRGB, this));
  this->server.on("/rgb", HTTP_GET, std::bind(&Api::conRGB, this));
  this->server.on("/device", HTTP_POST, std::bind(&Api::conSetDevice, this));
  this->server.on("/device", HTTP_GET, std::bind(&Api::conDevice, this));

  this->server.begin();
}
void Api::loop()
{
  this->server.handleClient();
}
void Api::loadRGB(StaticJsonDocument<256> json)
{
  if (json.isNull())
  {
    Serial.println("API: No rgb config");
    return;
  }
  byte r = json["r"];
  byte g = json["g"];
  byte b = json["b"];
  this->device.setAll(r, g, b);
  byte a = json["a"];
  this->device.setBrightness(a);
}
void Api::loadDevice(StaticJsonDocument<256> json)
{
  if (json.isNull())
  {
    Serial.println("API: No device config");
    return;
  }
  uint16_t leds = json["leds"];
  uint16_t pin = json["pin"];
  this->device.update(leds, pin);

  StaticJsonDocument<256> rgb = this->getRGB();
  this->loadRGB(rgb);
}
void Api::conWeb()
{
  this->server.send_P(200, "text/html", WEB_HTML);
}
void Api::conToggle()
{
  digitalWrite(this->ledPin, !digitalRead(this->ledPin));
  this->server.send(204, "");
}
void Api::conSetRGB()
{
  StaticJsonDocument<256> json = this->getJson();
  this->loadRGB(json);
  String value;
  serializeJson(json, value);
  this->storage.put("rgb.json", value);

  this->server.send(200, "application/json", value.c_str());
}
void Api::conRGB()
{
  StaticJsonDocument<256> json = this->getRGB();
  String value;
  serializeJson(json, value);

  this->server.send(200, "application/json", value.c_str());
}
void Api::conSetDevice()
{
  StaticJsonDocument<256> json = this->getJson();
  this->loadDevice(json);

  String value;
  serializeJson(json, value);
  this->storage.put("device.json", value);

  this->server.send(200, "application/json", value.c_str());
}
void Api::conDevice()
{
  StaticJsonDocument<256> json = this->getDevice();

  String value;
  serializeJson(json, value);

  this->server.send(200, "application/json", value.c_str());
}
void Api::conSetWifi()
{
  StaticJsonDocument<256> json = this->getJson();
  String value;
  serializeJson(json, value);
  this->storage.put("wifi.json", value);

  server.send(200, "application/json", value.c_str());

  delay(500);
  Serial.println("API: Rebooting");
  ESP.restart(); //reload wifi
}
void Api::conWifi()
{
  StaticJsonDocument<256> json = this->getWifi();

  String value;
  serializeJson(json, value);

  this->server.send(200, "application/json", value.c_str());
}