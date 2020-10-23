#include "wifi.h"
#include "api.h"

WifiService WIFI = WifiService(LED_BUILTIN);
Api *API = new Api(LED_BUILTIN);

void setup()
{
  Serial.begin(115200);

  WIFI.setup();
  API->setup();
}

void loop()
{
  API->loop();
}