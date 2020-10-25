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
  Storage storage;
  byte ledPin;
  Device device;

  StaticJsonDocument<256> getJson();
  StaticJsonDocument<256> getRGB();
  StaticJsonDocument<256> getDevice();
  StaticJsonDocument<256> getWifi();
public:
  Api(byte pinLed);
  void setup();
  void loop();
  void loadRGB(StaticJsonDocument<256> json);
  void loadDevice(StaticJsonDocument<256> json);
  void conWeb();
  void conToggle();
  void conSetRGB();
  void conRGB();
  void conSetDevice();
  void conDevice();
  void conSetWifi();
  void conWifi();
};

#endif