#ifndef API_H
#define API_H

#include <ESP8266WebServer.h>
#include <ArduinoJson.h>

#include "storage.h"
#include "web.h"
#include "device.h"

class Api
{
private:
  ESP8266WebServer server;
  Storage *_storage;
  uint8_t _ledPin;
  Device *_device;

  StaticJsonDocument<128> getJson()
  {
    String data = server.arg("plain");
    StaticJsonDocument<128> json;
    DeserializationError err = deserializeJson(json, data);
    if (err)
    {
      Serial.print("### ERR: API - ");
      Serial.println(err.c_str());
    }
    return json;
  }

  StaticJsonDocument<128> getRGB()
  {
    String data = this->_storage->get("rgb.json");
    StaticJsonDocument<128> json;
    DeserializationError err = deserializeJson(json, data);
    if (err)
    {
      Serial.print("### ERR: API - ");
      Serial.println(err.c_str());
    }
    return json;
  }

public:
  Api(uint8_t pinLed)
  {
    this->_ledPin = pinLed;
    this->_storage = new Storage();
    this->_device = new Device(118, D4);
  }

  void setup()
  {
    pinMode(this->_ledPin, OUTPUT);

    this->server.on("/", HTTP_GET, std::bind(&Api::web, this));
    this->server.on("/toggle", HTTP_POST, std::bind(&Api::toggle, this));
    this->server.on("/wifi", HTTP_POST, std::bind(&Api::setWifi, this));
    this->server.on("/rgb", HTTP_POST, std::bind(&Api::setRGB, this));
    this->server.on("/device", HTTP_POST, std::bind(&Api::setDevice, this));

    server.begin();
  }

  void loop()
  {
    this->server.handleClient();
  }
  void web()
  {
    this->server.send_P(200, "text/html", WEB_HTML);
  }
  void toggle()
  {
    digitalWrite(this->_ledPin, !digitalRead(this->_ledPin));
    this->server.send(204, "");
  }
  void setRGB()
  {
    StaticJsonDocument<128> json = this->getJson();
    this->loadRGB(json);

    String value;
    serializeJson(json, value);
    this->_storage->put("rgb.json", value);

    this->server.send(204, "");
  }
  void loadRGB(StaticJsonDocument<128> json)
  {
    if (json.isNull())
    {
      Serial.println("API: No rgb config");
      return;
    }
    byte r = json["r"];
    byte g = json["g"];
    byte b = json["b"];
    this->_device->setAll(r, g, b);
    byte a = json["a"];
    this->_device->setBrightness(a);
  }
  void setDevice()
  {
    StaticJsonDocument<128> json = this->getJson();
    this->loadDevice(json);

    String value;
    serializeJson(json, value);
    this->_storage->put("device.json", value);

    this->server.send(204, "");
  }
  void loadDevice(StaticJsonDocument<128> json)
  {
    if (json.isNull())
    {
      Serial.println("API: No device config");
      return;
    }
    int leds = json["leds"];
    int pin = json["pin"];
    this->_device = new Device(leds, pin);

    StaticJsonDocument<128> rgb = this->getRGB();
    this->loadRGB(rgb);
  }
  void setWifi()
  {
    StaticJsonDocument<128> json = this->getJson();
    String value;
    serializeJson(json, value);
    this->_storage->put("wifi.json", value);

    server.send(200, "application/json", "{\"status\" : \"OK\"}");

    delay(500);
    Serial.println("API: Rebooting");
    ESP.restart(); //reload wifi
  }
};

#endif