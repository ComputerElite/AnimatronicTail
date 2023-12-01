
#include <Arduino.h>
#include "html.h"

const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html>
<head>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <style>
    html {
     font-family: Arial;
     display: inline-block;
     margin: 0px auto;
     text-align: center;
    }
    h2 { font-size: 3.0rem; }
  </style>
</head>
<body>
  <h2>ESP8266 for Tail</h2>
  <h3>Animations</h3>
  <div id="animations">

  </div>
  <h3>LEDs</h3>
  <div id="leds">

  </div>
  <h3>Positions</h3>
  <div id="positions">

  </div>
  <button onclick="sendPositions()">Send Positions</button>
  <h3>Speed</h3>
  <input type="number" onchange="sendSpeed()" min="1" max="70" value="40" id="speed">
  <h3>WiFi</h3>
  <label>SSID<input type="text" id="ssid"></label><br>
  <label>Password<input type="text" id="password"></label><br>
  <i id="wifiStatus"></i><br>
  <button onclick="sendWiFi()">Send WiFi</button>
</body>
<script>
    const ssid = document.getElementById("ssid");
    const password = document.getElementById("password");
    const speedRange = document.getElementById("speed");
    const animations = [
        {
            name: "Rest low",
            value: 0,
            speed: 30
        },
        {
            name: "Rest mid",
            value: 5,
            speed: 30
        },
        {
            name: "Rest high",
            value: 6,
            speed: 30
        },
        {
            name: "V Wag",
            value: 1,
            speed: 30
        },
        {
            name: "Wag",
            value: 2,
            speed: 30
        },
        {
            name: "Excited Wag",
            value: 3,
            speed: 50
        },
        {
            name: "Up and down",
            value: 4,
            speed: 20
        },
    ]

    var animationHtml = ``;
    animations.forEach((animation) => {
        animationHtml += `<button onclick="sendAnimation(${animation.value}, ${animation.speed})">${animation.name}</button>`
    })
    document.getElementById("animations").innerHTML = animationHtml;

    const leds = [
        {
            name: "Rainbow Fade",
            value: 0
        }
    ]

    var ledHTML = ``;
    leds.forEach((animation) => {
        ledHTML += `<button onclick="sendLED(${animation.value})">${animation.name}</button>`
    })
    document.getElementById("leds").innerHTML = ledHTML;

    function UpdatePositions() {
        fetch("/positions")
            .then((response) => {
                return response.json();
            })
            .then((data) => {
                var positionHTML = ``;
                for(const [key, value] of Object.entries(data)) {
                    positionHTML += `<label>${key}<input class="positions" type="number" value=${value}></input></label><br>      `
                }
                document.getElementById("positions").innerHTML = positionHTML;
            })
    }

    UpdatePositions();

    function sendPositions() {
        var positions = document.getElementsByClassName("positions");
        var body = {};
        for(var i = 0; i < positions.length; i++) {
            body[positions[i].labels[0].innerText] = parseInt(positions[i].value);
        }
        alert(JSON.stringify(body));
        fetch(`/positions`, {
            method: "POST",
            body: JSON.stringify(body)
        })
    }

    function sendLED(led) {
        fetch(`/led`, {
            method: "POST",
            body: led
        })
    }

    function sendSpeed() {
        fetch(`/speed`, {
            method: "POST",
            body: speedRange.value
        })
    }

    function sendAnimation(animation, speed) {
        fetch(`/animation`, {
            method: "POST",
            body: animation
        })
        speedRange.value = speed
        fetch(`/speed`, {
            method: "POST",
            body: speed
        })
    }

    UpdateWifi()
    function UpdateWifi() {
        fetch("/wifi")
            .then((response) => {
                return response.json();
            })
            .then((data) => {
                ssid.value = data.ssid;
                password.value = data.password;
                document.getElementById("wifiStatus").innerText = data.status;
            })
    
    }

    function sendWiFi() {
        fetch(`/wifi`, {
            method: "POST",
            body: JSON.stringify({
                ssid: ssid.value,
                password: password.value
            })
        })
        alert("WiFi sent. ESP will try to connect to it. Keep your eye out for a new network device. If it fails the setup network will open again.")
    }
</script>
</html>
)rawliteral";