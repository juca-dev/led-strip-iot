#include "api.h"

const char ERR_400[] = "{\"error\":\"invalid params\"}";
const char ERR_405[] = "{\"error\":\"api power OFF\"}";

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
StaticJsonDocument<256> Api::config()
{
  String data = this->storage.get("api.json");
  StaticJsonDocument<256> json;
  DeserializationError err = deserializeJson(json, data);
  if (err)
  {
    Serial.print("### ERR: API - ");
    Serial.println(err.c_str());
  }
  return json;
}

Api::Api(byte ledPin, byte powerPin)
{
  this->ledPin = ledPin;
  this->powerPin = powerPin;
  Serial.println("API: ready");
}

void Api::setup()
{
  pinMode(this->ledPin, OUTPUT);
  pinMode(this->powerPin, INPUT);
  delay(250);
  this->powerBkp = digitalRead(this->powerPin);

  this->wifi.setup();

  this->storage.setup();

  this->device.setup();

  this->configBkp = this->config();
  this->load(this->configBkp);

  this->server.on("/", HTTP_GET, std::bind(&Api::conWeb, this));
  this->server.on("/", HTTP_DELETE, std::bind(&Api::conReset, this));
  this->server.on("/toggle", HTTP_POST, std::bind(&Api::conToggle, this));
  this->server.on("/wifi", HTTP_POST, std::bind(&Api::conWifiPut, this));
  this->server.on("/wifi", HTTP_GET, std::bind(&Api::conWifi, this));
  this->server.on("/rgb", HTTP_POST, std::bind(&Api::conConfigPut, this));
  this->server.on("/rgb", HTTP_GET, std::bind(&Api::conConfig, this));
  this->server.on("/device", HTTP_POST, std::bind(&Api::conDevicePut, this));
  this->server.on("/device", HTTP_GET, std::bind(&Api::conDevice, this));

  this->server.begin();
}
void Api::loop()
{
  this->server.handleClient();
  this->checkPower();
}
void Api::checkPower()
{
  if (this->powerBkp == digitalRead(this->powerPin))
  {
    return;
  }

  this->powerBkp = !this->powerBkp;
  Serial.print("API: power ");
  Serial.println(this->powerBkp);

  if (this->powerBkp)
  {
    this->device.clear();
  }
  else
  {
    this->load(this->configBkp);
  }
}
bool Api::load(StaticJsonDocument<256> json)
{
  if (json.isNull() || !json.containsKey("r") || !json.containsKey("g") || !json.containsKey("b") || !json.containsKey("a"))
  {
    Serial.println("API: No rgb config");
    return false;
  }

  byte r = json["r"];
  byte g = json["g"];
  byte b = json["b"];
  this->device.setAll(r, g, b);
  byte a = json["a"];
  this->device.setBrightness(a);

  String value;
  serializeJson(json, value);

  this->storage.put("api.json", value);
  this->configBkp = json;

  return true;
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
void Api::conConfigPut()
{
  if (this->powerBkp)
  {
    this->server.send(405, "application/json", ERR_405);
    return;
  }
  StaticJsonDocument<256> json = this->getJson();
  if (!this->load(json))
  {
    this->server.send(400, "application/json", ERR_400);
    return;
  }

  String value;
  serializeJson(json, value);

  this->server.send(204, "");
}
void Api::conConfig()
{
  StaticJsonDocument<256> json = this->config();
  String value;
  serializeJson(json, value);

  this->server.send(200, "application/json", value.c_str());
}
void Api::conDevicePut()
{
  if (this->powerBkp)
  {
    this->server.send(405, "application/json", ERR_405);
    return;
  }
  StaticJsonDocument<256> json = this->getJson();
  if (!this->device.load(json))
  {
    this->server.send(400, "application/json", ERR_400);
    return;
  }
  String value;
  serializeJson(json, value);

  json = this->config();
  this->load(json);

  this->server.send(204, "");
}
void Api::conDevice()
{
  StaticJsonDocument<256> json = this->device.config();

  String value;
  serializeJson(json, value);

  this->server.send(200, "application/json", value.c_str());
}
void Api::conWifiPut()
{
  StaticJsonDocument<256> json = this->getJson();
  String value;
  serializeJson(json, value);

  server.send(204, "");

  wifi.load(json);
}
void Api::conWifi()
{
  StaticJsonDocument<256> json = this->wifi.config();

  String value;
  serializeJson(json, value);

  this->server.send(200, "application/json", value.c_str());
}
void Api::conReset()
{
  server.send(204, "");

  delay(500);
  Serial.println("API: Rebooting");
  ESP.restart(); //reload wifi
}