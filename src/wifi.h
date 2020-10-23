#ifndef WIFISERVICE_H
#define WIFISERVICE_H

#include <ESP8266WiFi.h>
#include <ArduinoJson.h>

#include "storage.h"

const char *WIFI_PWD = "123123123";
const char *WIFI_SSID = "juca.dev";

IPAddress WIFI_LOCAL_IP(192, 168, 100, 4);
IPAddress WIFI_GATEWAY(192, 168, 100, 1);
IPAddress WIFI_NETMASK(255, 255, 255, 0);

class WifiService
{
private:
    uint8_t _ledPin;
    Storage *_storage;

public:
    WifiService(uint8_t pin)
    {
        this->_ledPin = pin;
        this->_storage = new Storage();
    }
    void setup()
    {
        //reset networking
        WiFi.softAPdisconnect(true);
        WiFi.disconnect();
        delay(1000);
        //check for stored credentials
        String config = this->_storage->get("wifi.json");
        if (config)
        {
            Serial.println("WIFI: has config file");

            StaticJsonDocument<128> json;
            DeserializationError err = deserializeJson(json, config);
            if (err)
            {
                Serial.print("### ERR: Wifi deserializeJson - ");
                Serial.println(err.c_str());
                return;
            }

            Serial.print("WIFI: ");
            serializeJson(json, Serial);
            Serial.println("");

            const char *ssid = "", *pwd = "";
            ssid = json["ssid"];
            pwd = json["password"];
            WiFi.mode(WIFI_STA);
            WiFi.begin(ssid, pwd);
            unsigned long startTime = millis();
            Serial.print("WIFI: Wait");
            while (WiFi.status() != WL_CONNECTED)
            {
                delay(500);
                Serial.print(".");
                digitalWrite(this->_ledPin, !digitalRead(this->_ledPin));
                if ((unsigned long)(millis() - startTime) >= 15000)
                    break;
            }
            Serial.println("");
        }
        else
        {
            Serial.println("### ERR: Wifi config: NOK");
        }

        if (WiFi.status() == WL_CONNECTED)
        {
            digitalWrite(this->_ledPin, HIGH);
            Serial.println("WIFI: OK");
            Serial.println(WiFi.localIP());
        }
        else
        {
            WiFi.mode(WIFI_AP);
            WiFi.softAPConfig(WIFI_LOCAL_IP, WIFI_GATEWAY, WIFI_NETMASK);
            WiFi.softAP(WIFI_SSID, WIFI_PWD);
            digitalWrite(this->_ledPin, LOW);
            Serial.println("Connected: NOK");
        }
        Serial.println("");
        WiFi.printDiag(Serial);
    }
};

#endif