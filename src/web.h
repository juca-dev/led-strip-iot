const char WEB_HTML[] PROGMEM = R"=====(
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
        <button class="primary" id="savebtn" type="button" onclick="settings()">SAVE</button>
      </div>
    </fieldset>
  </form>
  <form>
    <fieldset>
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
function settings()
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
function setRGB()
{
  console.log("rgb was clicked!");

  var r = +document.getElementById("color_r").value;
  var g = +document.getElementById("color_g").value;
  var b = +document.getElementById("color_b").value;
  var a = +document.getElementById("color_a").value;
  var data = {r, g, b, a};
  document.body.style.backgroundColor = 'rgb(' + r + ',' + g + ',' + b + ',' + (a/255) + ')';

  var xhr = new XMLHttpRequest();
  var url = "/rgb";

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