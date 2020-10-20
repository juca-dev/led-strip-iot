/*------------------------------------------------------------------------------
  10/2/2018
  Author: Makerbro
  Platforms: ESP8266
  Language: C++/Arduino
  File: wifi_modes_switch.ino
  ------------------------------------------------------------------------------
  Description: 
  Code for YouTube video demonstrating how  to switch the ESP8266 between Station 
  (STA) and Access Point (AP) modes, and how to store network credentials in 
  Flash memory:
  https://youtu.be/lyoBWH92svk

  Do you like my videos? You can support the channel:
  https://patreon.com/acrobotic
  https://paypal.me/acrobotic
  ------------------------------------------------------------------------------
  Please consider buying products from ACROBOTIC to help fund future
  Open-Source projects like this! We'll always put our best effort in every
  project, and release all our design files and code for you to use. 

  https://acrobotic.com/
  https://amazon.com/acrobotic
  ------------------------------------------------------------------------------
  License:
  Please see attached LICENSE.txt file for details.
------------------------------------------------------------------------------*/
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <FS.h>
#include <ArduinoJson.h>
#include <Adafruit_NeoPixel.h>

ESP8266WebServer server;
uint8_t PIN_LED = LED_BUILTIN;
char *WIFI_PWD = "123123123";
char *WIFI_SSID = "JUCA.DEV";

IPAddress LOCAL_IP(192, 168, 100, 4);
IPAddress GATEWAY(192, 168, 100, 1);
IPAddress NETMASK(255, 255, 255, 0);

char HTML[] PROGMEM = R"=====(
<html>
<head>
</head>
<body>
  <form>
    <fieldset>
      <div>
        <label for="ssid">SSID</label>      
        <input value="" id="ssid" placeholder="SSID">
      </div>
      <div>
        <label for="password">PASSWORD</label>
        <input type="password" value="" id="password" placeholder="PASSWORD">
      </div>
      <div>
        <button class="primary" id="savebtn" type="button" onclick="myFunction()">SAVE</button>
      </div>
    </fieldset>
  </form>
</body>
<script>
function myFunction()
{
  console.log("button was clicked!");

  var ssid = document.getElementById("ssid").value;
  var password = document.getElementById("password").value;
  var data = {ssid:ssid, password:password};

  var xhr = new XMLHttpRequest();
  var url = "/settings";

  xhr.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      // Typical action to be performed when the document is ready:
      if(xhr.responseText != null){
        alert(xhr.responseText);
      }
    }
  };

  xhr.open("POST", url, true);
  xhr.send(JSON.stringify(data));
};
</script>
</html>
)=====";

class Device
{
private:
  int leds;
  byte brightness;
  Adafruit_NeoPixel device;

public:
  Device(int leds, int pin, byte brightness)
  {
    this->leds = leds;
    this->brightness = brightness;

    this->device = Adafruit_NeoPixel(leds, pin, NEO_GRB + NEO_KHZ800);
    this->device.begin();

    this->setBrightness(brightness);

    this->clear();
  }
  void clear()
  {
    for (int i = 0; i < this->leds; i++)
    {
      this->device.setPixelColor(i, this->device.Color(0, 0, 0)); //change RGB color value here
    }
    this->device.show();
  }
  void setBrightness(byte value)
  {
    this->device.setBrightness(value);
  }
  void setAll(byte r, byte g, byte b)
  {
    for (int i = 0; i < this->leds; i++)
    {
      this->device.setPixelColor(i, this->device.Color(r, g, b)); //change RGB color value here
    }
    this->device.show();
  }
  int *split(String value)
  {
    int result[(this->leds * 3) + 1];

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
      this->device.setPixelColor(i, this->device.Color(r, g, b)); //change RGB color value here
    }
    this->device.show();
  }
};
Device LED_STRIP = Device(32, D4, 254);

void setup()
{
  pinMode(PIN_LED, OUTPUT);
  Serial.begin(115200);
  SPIFFS.begin();

  wifiConnect();

  server.on("/", []() { server.send_P(200, "text/html", HTML); });
  server.on("/toggle", toggleLED);
  server.on("/settings", HTTP_POST, handleSettingsUpdate);
  // server.on("/rgb", HTTP_POST, handleSettingsUpdate);

  server.begin();
}

void loop()
{
  server.handleClient();
}

void handleSettingsUpdate()
{
  String data = server.arg("plain");
  StaticJsonDocument<128> jObject;
  DeserializationError err = deserializeJson(jObject, data);
  if (err)
  {
    Serial.print("Error Config - deserializeJson: ");
    Serial.println(err.c_str());
    server.send(200, "application/json", "{\"status\" : \"NOK\"}");
    return;
  }
  else
  {
    Serial.print("Data: ");
    serializeJson(jObject, Serial);
    Serial.println("");
  }

  File configFile = SPIFFS.open("/config.json", "w");
  serializeJson(jObject, configFile);
  configFile.close();

  server.send(200, "application/json", "{\"status\" : \"OK\"}");
  delay(500);

  wifiConnect();
}

void wifiConnect()
{
  //reset networking
  WiFi.softAPdisconnect(true);
  WiFi.disconnect();
  delay(1000);
  //check for stored credentials
  if (SPIFFS.exists("/config.json"))
  {
    Serial.println("configFile: OK");
    const char *_ssid = "", *_pass = "";
    File configFile = SPIFFS.open("/config.json", "r");
    if (configFile)
    {
      size_t size = configFile.size();
      std::unique_ptr<char[]> buf(new char[size]);
      configFile.readBytes(buf.get(), size);
      configFile.close();

      StaticJsonDocument<128> jObject;
      DeserializationError err = deserializeJson(jObject, buf.get());
      if (err)
      {
        Serial.print("Error configFile - deserializeJson: ");
        Serial.println(err.c_str());
        return;
      }
      else
      {
        Serial.print("ConfigFile: ");
        serializeJson(jObject, Serial);
        Serial.println("");

        _ssid = jObject["ssid"];
        _pass = jObject["password"];
        WiFi.mode(WIFI_STA);
        WiFi.begin(_ssid, _pass);
        unsigned long startTime = millis();
        Serial.print("Wait");
        while (WiFi.status() != WL_CONNECTED)
        {
          delay(500);
          Serial.print(".");
          digitalWrite(PIN_LED, !digitalRead(PIN_LED));
          if ((unsigned long)(millis() - startTime) >= 15000)
            break;
        }
        Serial.println("");

        Serial.println("Led configured");
      }
    }
  }
  else
  {
    Serial.println("configFile: NOK");
  }

  if (WiFi.status() == WL_CONNECTED)
  {
    digitalWrite(PIN_LED, HIGH);
    Serial.println("Connected: OK");
    Serial.println(WiFi.localIP());
  }
  else
  {
    WiFi.mode(WIFI_AP);
    WiFi.softAPConfig(LOCAL_IP, GATEWAY, NETMASK);
    WiFi.softAP(WIFI_SSID, WIFI_PWD);
    digitalWrite(PIN_LED, LOW);
    Serial.println("Connected: NOK");
  }
  Serial.println("");
  WiFi.printDiag(Serial);
}

void toggleLED()
{
  digitalWrite(PIN_LED, !digitalRead(PIN_LED));
  LED_STRIP.setAll(0, 254, 0);
  server.send(204, "");
}