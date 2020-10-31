<p align="center">
  <!-- <img src="https://storage.googleapis.com/golden-wind/unform/unform.svg" height="150" width="175" alt="Unform" /> -->
</p>

<h3 align="center">
  Easy [led strip](https://en.wikipedia.org/wiki/LED_strip_light) controller! 🚀
</h3>

<br>

# Overview
IoT Led controller to WS2812 5050 (any addressable  rgb led strip)

## Tested devices
 - NodeMCU 1.0
 - Wemos D1 R1

## Libraries 
Access `Sketch > Include Library > Manage Libraries...`, looking for:
 - [ArduinoJson](https://arduinojson.org/v6/example/) - v6.17.0 (by *Benoit Banchon*)
 - [Adafruit NeoPixel](https://adafruit.github.io/Adafruit_NeoPixel/html/class_adafruit___neo_pixel.html) - v1.5.0 (by *Adafruit*)

## Settings
 - **Enable Flash (SPIFFS)** `Tools > Flash Size > 4M (1M SPIFFS)`
 - **Wifi low memory** `Tools > IwIP Variant > v2 Lower Memory`
 - **Write speed** `Tools > Upload Speed > 921600`

 ## Connect Wifi
 Set wifi device connection
  - Looking for *wifi* `JUCA.DEV` 
  - Enter the *password* `123123123` 
  - Access configuration page [http://192.168.100.4](http://192.168.100.4)
  - Input your wifi credentials (`SSID` and `Password`)
  - Ready, wifi configured

 ## Endpoints
 Access [http://leds.juca.dev](http://leds.juca.dev) to control led
  - `GET /` html page
  - `GET /toggle` enable/disable *device led*
  - `POST /wifi` update wifi credentials
    ```json
    {
        "ssid": "<WIFI_NAME>",
        "password": "<WIFI_PASSWORD>"
    }
    ```
  - `POST /rgb` update color (0-254)
    ```json
    {
        "r": 0,
        "g": 0,
        "b": 120,
        "a": 254
    }
    ```
  - `POST /device` update device
    ```json
    {
        "pin": 2,
        "leds": 148
    }
    ```

## Need help?

If you need help, feel free to open an issue with a description of the problem you're facing. Or, if you prefer, you can send email to [github@juca.dev](mailto:github@juca.dev)

## License

MIT © [Juca.DEV](https://juca.dev)