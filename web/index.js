function connect(val) {
  var url = `http://192.168.100.${val}`;
  var xhr = new XMLHttpRequest();

  xhr.onreadystatechange = () => {
    if (xhr.readyState == 4) {
      console.log(
        `${url} - ${xhr.readyState} - ${xhr.status} ${this.readyState} ${this.status} ${xhr.responseText}`
      );
      if (xhr.status >= 200 && xhr.status < 304) {
        console.log(`connection exists => ${url}`);
      }
    }
  };
  xhr.onload = () => {
    console.log("### LOAD");
  };

  xhr.open("GET", url);
  xhr.setRequestHeader("Access-Control-Allow-Origin", "*");
  xhr.send();
}

function test() {
  console.log(`### START`);
  // for (let i = 0; i < 255; i++) {
  //   connect(i);
  // }
  connect(121);

  console.log(`### END`);
}
test();
