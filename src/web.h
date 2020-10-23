const char WEB_HTML[] PROGMEM = R"=====(
<html>
<head>
</head>
<body>
  <form>
    <fieldset>
      <legend>WIFI</legend>
      <div>
        <label for="wifi_ssid">SSID</label>      
        <input value="" id="wifi_ssid" placeholder="SSID">
      </div>
      <div>
        <label for="wifi_password">PASSWORD</label>
        <input type="wifi_password" value="" id="wifi_password" placeholder="PASSWORD">
      </div>
      <div>
        <button class="primary" id="savebtn" type="button" onclick="setWifi()">Connect</button>
      </div>
    </fieldset>
  </form>
  <form>
    <fieldset>
      <legend>RGB</legend>
      <div>
        <label for="color_r">R</label>      
        <input type="range" min="0" max="255" value="0" id="color_r" onchange="setRGB()" />
      </div>
      <div>
        <label for="color_g">G</label>      
        <input type="range" min="0" max="255" value="0" id="color_g" onchange="setRGB()" />
      </div>
      <div>
        <label for="color_b">B</label>      
        <input type="range" min="0" max="255" value="0" id="color_b" onchange="setRGB()" />
      </div>
      <div>
        <label for="color_a">A</label>      
        <input type="range" min="0" max="255" value="125" id="color_a" onchange="setRGB()" />
      </div>
    </fieldset>
  </form>
</body>
<script>
function setWifi()
{
  console.log("wifi were updated!");

  var ssid = document.getElementById("wifi_ssid").value;
  var password = document.getElementById("wifi_password").value;
  var data = {ssid:ssid, password:password};

  var xhr = new XMLHttpRequest();
  var url = "/wifi";

  xhr.onreadystatechange = () => {
    if (this.readyState == 4 && this.status == 200) {
      if(xhr.responseText != null){
        alert(xhr.responseText);
      }
    }
  };

  xhr.open("POST", url, true);
  xhr.send(JSON.stringify(data));
};
function setRGB()
{
  console.log("rgb were updated!");

  var r = +document.getElementById("color_r").value;
  var g = +document.getElementById("color_g").value;
  var b = +document.getElementById("color_b").value;
  var a = +document.getElementById("color_a").value;
  var data = {r, g, b, a};
  document.body.style.backgroundColor = 'rgb(' + r + ',' + g + ',' + b + ',' + (a/255) + ')';

  var xhr = new XMLHttpRequest();
  var url = "/rgb";

  xhr.onreadystatechange = () => {
    if (this.readyState == 4 && this.status == 200) {
      if(xhr.responseText != null){
        alert(xhr.responseText);
      }
    }
  };

  xhr.open("POST", url, true);
  xhr.send(JSON.stringify(data));
};
function setDevice()
{
  console.log("device were updated!");

  var pin = +document.getElementById("device_pin").value;
  var leds = +document.getElementById("device_leds").value;
  var data = {pin, leds};

  var xhr = new XMLHttpRequest();
  var url = "/device";

  xhr.onreadystatechange = () => {
    if (this.readyState == 4 && this.status == 200) {
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