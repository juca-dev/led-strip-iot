const char WEB_HTML[] PROGMEM = R"=====(
<!DOCTYPE html>
<html lang="en">
  <head>
    <meta charset="UTF-8" />
    <meta name="viewport" content="width=device-width, initial-scale=1.0" />
    <title>Led Controller</title>
    <link href="https://fonts.googleapis.com/css2?family=Poppins:wght@300;500&display=swap" rel="stylesheet">
    <style>
      * {
        margin: 0;
        padding: 0;
        border: none;
        box-sizing: border-box;
      }
      *,
      input,
      button {
        outline: none;
        font-family: "Poppins", sans-serif;
        box-sizing: border-box;
      }
      body {
        display: flex;
        flex-direction: column;
        align-items: center;
        justify-content: space-evenly;
        min-height: 100vh;
        background: #000;
      }
      main {
        position: relative;
        padding: 2.5em;
        background: #f3f3f3;
        border-radius: 0.5em;
        box-shadow: 0 0.5em 1.5em rgba(0, 0, 0, 0.5);
      }
      h1 {
        color: #000;
        text-shadow: 0 0 0.25em rgba(255, 255, 255, 0.75);
        font-size: xx-large;
      }
      fieldset {
        margin-bottom: 1em;
        padding: 1em;
        background: #fff;
        border-radius: 0.5em;
      }
      fieldset > div {
        position: relative;
        width: 50vw;
        height: 3em;
        margin-bottom: 1.5em;
      }
      fieldset > div:last-child {
        margin-bottom: 0;
      }
      fieldset > div > input {
        position: absolute;
        top: 0;
        left: 0;
        width: 100%;
        border: 1px solid #000;
        background: transparent;
        padding: 1em;
        border-radius: 0.5em;
        font-size: medium;
        color: #000;
        font-weight: 300;
      }
      fieldset > div > label {
        position: absolute;
        top: 1px;
        left: 1px;
        padding: 1em;
        display: inline-block;
        font-size: medium;
        color: #000;
        font-weight: 300;
        transition: 0.5s;
        pointer-events: none;
      }
      fieldset > div > input:focus ~ label,
      fieldset > div > input:valid ~ label {
        transform: translateX(-1em) translateY(-2.25em);
        font-size: small;
      }
      fieldset > div > input[type="submit"] {
        background: #2196f3;
        color: #fff;
        max-width: 50%;
        cursor: pointer;
        font-weight: 500;
        border: none;
      }
      fieldset > div > input[type="submit"]:hover {
        background: #e91e63;
      }
      fieldset > div > input[type="color"] {
        height: 3em;
        padding: 0;
      }
      legend {
        height: 1.5em;
        font-weight: 500;
        font-size: larger;
        padding-left: 0.5em;
        border-left: 0.25em solid #e91e63;
        margin-bottom: 1.5em;
      }
      .hide {
        display: none;
      }
      #loader {
        position: relative;
        width: 200px;
        height: 200px;
      }
      #loader::before {
        content: "";
        position: absolute;
        top: 85px;
        left: 15%;
        width: 70%;
        height: 70%;
        background: #ff3b8d;
        box-shadow: -60px -55px 0 #07b2ff;
        border-radius: 50%;
        animation: animate 2.5s linear infinite;
      }
      #loader::after {
        content: "";
        position: absolute;
        bottom: 85px;
        left: 15%;
        width: 70%;
        height: 70%;
        background: #8dff08;
        box-shadow: 60px 55px 0 #ffeb3b;
        border-radius: 50%;
        animation: animate 2.5s linear infinite;
      }
      #loader > span {
        position: absolute;
        top: 0;
        left: 0;
        width: 100%;
        height: 100%;
        background: #fff;
        border-radius: 50%;
        z-index: 1;
        display: flex;
        align-items: center;
        justify-content: center;
        text-transform: uppercase;
        letter-spacing: 0.5em;
        font-weight: 500;
        font-size: large;
        color: rgba(0, 0, 0, 0.1);
      }
      #loader > span::before {
        content: "";
        position: absolute;
        top: 8px;
        left: 8px;
        right: 8px;
        bottom: 8px;
        background: transparent;
        border-radius: 50%;
        box-shadow: inset 0 0 20px rgba(0, 0, 0, 0.25);
      }
      @keyframes animate {
        0% {
          filter: blur(50px) hue-rotate(0deg);
        }
        100% {
          filter: blur(50px) hue-rotate(360deg);
        }
      }
      #loader ~ main {
        display: none;
      }
    </style>
  </head>
  <body>
    <header>
      <h1>Led Controller</h1>
    </header>
    <div id="loader">
      <span>Loading</span>
    </div>
    <main>
      <form onsubmit="setWifi()">
        <fieldset>
          <legend>WIFI</legend>
          <div>
            <input type="text" required id="wifi_ssid" placeholder="SSID" />
            <label for="wifi_ssid">SSID</label>
          </div>
          <div>
            <input
              type="text"
              required
              id="wifi_password"
              placeholder="PASSWORD"
            />
            <label for="wifi_password">PASSWORD</label>
          </div>
          <div>
            <input type="submit" value="Connect" />
          </div>
        </fieldset>
      </form>
      <form>
        <fieldset>
          <legend>Color</legend>
          <div>
            <input type="color" id="color_rgb" oninput="setRGB()" />
            <label for="color_rgb">RGB</label>
          </div>
          <div>
            <input
              type="range"
              min="0"
              max="255"
              id="color_w"
              oninput="setRGB()"
            />
            <label for="color_w">Intensity</label>
          </div>
        </fieldset>
      </form>
    </main>
  </body>
  <script>
    const wifi_ssid = document.getElementById("wifi_ssid"),
      wifi_password = document.getElementById("wifi_password"),
      color_rgb = document.getElementById("color_rgb"),
      color_w = document.getElementById("color_w");

    function hexToRGB(hex) {
      let result = /^#?([a-f\d]{2})([a-f\d]{2})([a-f\d]{2})$/i.exec(hex);
      return result
        ? {
            r: parseInt(result[1], 16),
            g: parseInt(result[2], 16),
            b: parseInt(result[3], 16),
          }
        : null;
    }
    function rgbToHex(data) {
      return (
        "#" +
        ((1 << 24) + (data.r << 16) + (data.g << 8) + data.b)
          .toString(16)
          .slice(1)
      );
    }
    async function request(method, path, data) {
      let xhr = new XMLHttpRequest();
      return await new Promise((resolve, reject) => {
        xhr.onreadystatechange = () => {
          if (xhr.readyState == 4) {
            if (xhr.status < 200 || xhr.status >= 300) {
              console.error(xhr);
              return reject(xhr.responseText || `Status: ${xhr.status}`);
            }
            return resolve(JSON.parse(xhr.responseText));
          }
        };

        xhr.open(method, path, true);
        xhr.send(JSON.stringify(data));
      });
    }
    async function setWifi(ev) {
      if (ev) {
        ev.preventDefault();
      }
      console.log("wifi were updated!");

      let ssid = wifi_ssid.value;
      let password = wifi_password.value;
      let data = { ssid: ssid, password: password };

      request("POST", "/wifi", data);
    }
    async function setRGB() {
      console.log("rgb were updated!");

      let data = hexToRGB(color_rgb.value);
      data.a = +color_w.value;

      var value = `rgb(${data.r},${data.g},${data.b},${data.a / 255})`;
      document.body.style.backgroundColor = `${value}`;

      request("POST", "/rgb", data);
    }
    async function getRGB() {
      console.log("get rgb!");

      let res = await request("GET", "/rgb");

      color_rgb.value = rgbToHex(res);
      color_w.value = res.a;
      var value = `rgb(${res.r},${res.g},${res.b},${res.a / 255})`;
      document.body.style.backgroundColor = `${value}`;
    }
    async function getWifi() {
      console.log("get rgb!");

      let res = await request("GET", "/wifi");

      wifi_ssid.value = res.ssid;
      wifi_password.value = res.password;
    }
    async function setDevice() {
      console.log("device were updated!");

      let pin = +document.getElementById("device_pin").value;
      let leds = +document.getElementById("device_leds").value;
      let data = { pin, leds };

      request("POST", "/device", data);
    }
    window.onload = async () => {
      try {
        await getRGB();
        await getWifi();
      } catch (err) {
        alert(err);
      }

      setTimeout(() => {
        document.getElementById("loader").remove();
      }, 5000);
    };
  </script>
</html>
)=====";