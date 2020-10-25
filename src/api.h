#ifndef API_H
#define API_H

#include <ESP8266WebServer.h>
#include <ArduinoJson.h>

#include "storage.h"
#include "web.h"
#include "device.h"
#include "wifi.h"

class Api
{
private:
  ESP8266WebServer server;
  Storage storage;
  byte ledPin;
  Device device;
  WifiService wifi = WifiService(LED_BUILTIN);


  StaticJsonDocument<256> getJson();
  StaticJsonDocument<256> config();
public:
  Api(byte pinLed);
  void setup();
  void loop();
  bool load(StaticJsonDocument<256> json);
  void conWeb();
  void conToggle();
  void conConfig();
  void conConfigPut();
  void conDevice();
  void conDevicePut();
  void conWifi();
  void conWifiPut();
  void conReset();
};

#endif