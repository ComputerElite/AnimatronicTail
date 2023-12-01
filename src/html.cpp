
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
    button {
        background-color: #195B6A;
        border: none;
        color: white;
        text-decoration: none;
        margin: 2px;
        padding: 10px;
        cursor: pointer;
    }
    .grid {
        display: grid;
        grid-template-columns: repeat(3, 1fr);
        grid-gap: 10px;
    }
  </style>
<link rel="stylesheet" href="https://cdn.jsdelivr.net/npm/reinvented-color-wheel@0.1.6/css/reinvented-color-wheel.min.css">
<script src="https://cdn.jsdelivr.net/npm/reinvented-color-wheel@0.1.6"></script>
</head>
<body>
  <h2>ESP8266 for Tail</h2>
  <h3>Animations</h3>
  <div class="grid" id="animations">

  </div>
  <h3>Speed</h3>
  <input type="number" onchange="sendSpeed()" min="0" max="70" value="40" id="speed">
  <h3>LEDs</h3>
  <div class="grid" id="leds">

  </div>
  <h3>LED Speed</h3>
  <input type="number" onchange="sendLEDSpeed()" value="40" id="ledspeed">
  <h3>LED brightness</h3>
  <input type="range" onchange="sendLEDBrightness()" min="0" max="255" value="255" id="brightness">
  <h3>Color 0</h3>
  <div id="color0">

  </div>
  <h3>Positions</h3>
  <div id="positions">

  </div>
  <button onclick="sendPositions()">Send Positions</button>
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
    const ledSpeedRange = document.getElementById("ledspeed");
    const brightness = document.getElementById("brightness");
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

    picker = new ReinventedColorWheel({
                    appendTo: document.getElementById('color0'),
                    wheelDiameter: 500,
                    wheelThickness: 80,
                    handleDiameter: 70,
                    onChange: SetColor0
                })

    function SetColor0(color, update = true) {
        console.log(color);
        console.log(HSVtoRGB(color.hsv[0], color.hsv[1], color.hsv[2]));
        var c = HSVtoRGB(color.hsv[0], color.hsv[1], color.hsv[2]);
        var hex = ((c.r << 16) + (c.g << 8) + c.b).toString(16)
        if(update) {
            fetch(`/color0`, {
                method: "POST",
                body: hex
            });
        }
    }

    var animationHtml = ``;
    animations.forEach((animation) => {
        animationHtml += `<button onclick="sendAnimation(${animation.value}, ${animation.speed})">${animation.name}</button>`
    })
    document.getElementById("animations").innerHTML = animationHtml;

    UpdateLEDPatterns()
    function UpdateLEDPatterns() {
        fetch("/ledpatterns").then((response) => {
            return response.json();
        }).then((leds) => {
            var ledHTML = ``;
            leds.forEach((animation) => {
                ledHTML += `<button onclick="sendLED(${animation.id})">${animation.name}</button>`
            })
            document.getElementById("leds").innerHTML = ledHTML;
        })
    }

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

    function sendLEDBrightness() {
        fetch(`/brightness`, {
            method: "POST",
            body: brightness.value
        })
    }

    function sendSpeed() {
        fetch(`/speed`, {
            method: "POST",
            body: speedRange.value
        })
    }
    function sendLEDSpeed() {
        fetch(`/ledspeed`, {
            method: "POST",
            body: ledSpeedRange.value
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

    function RGBtoHSV(rgb) {
            var r = (rgb >> 16) & 255;
            var g = (rgb >> 8) & 255;
            var b = rgb & 255;

            r = r / 255; // Normalize r to the range of 0-1
            g = g / 255; // Normalize g to the range of 0-1
            b = b / 255; // Normalize b to the range of 0-1

            var max = Math.max(r, g, b);
            var min = Math.min(r, g, b);
            var delta = max - min;

            var h, s, v;

            // Calculate hue (h)
            if (delta === 0) {
                h = 0; // No difference, so hue is undefined (can be set to any value)
            } else if (max === r) {
                h = ((g - b) / delta) % 6;
            } else if (max === g) {
                h = (b - r) / delta + 2;
            } else {
                h = (r - g) / delta + 4;
            }

            h = (h * 60 + 360) % 360; // Convert h to the range of 0-360

            // Calculate saturation (s)
            s = max === 0 ? 0 : delta / max;

            // Calculate value (v)
            v = max;

            // Convert saturation and value to the range of 0-100
            s = Math.round(s * 100);
            v = Math.round(v * 100);

            return { h: h, s: s, v: v };
        }


        function HSVtoRGB(h, s, v) {
            var r, g, b;

            h = h / 360; // Normalize h to the range of 0-1
            s = s / 100; // Normalize s to the range of 0-1
            v = v / 100; // Normalize v to the range of 0-1

            var i = Math.floor(h * 6);
            var f = h * 6 - i;
            var p = v * (1 - s);
            var q = v * (1 - f * s);
            var t = v * (1 - (1 - f) * s);

            switch (i % 6) {
                case 0:
                    r = v;
                    g = t;
                    b = p;
                    break;
                case 1:
                    r = q;
                    g = v;
                    b = p;
                    break;
                case 2:
                    r = p;
                    g = v;
                    b = t;
                    break;
                case 3:
                    r = p;
                    g = q;
                    b = v;
                    break;
                case 4:
                    r = t;
                    g = p;
                    b = v;
                    break;
                case 5:
                    r = v;
                    g = p;
                    b = q;
                    break;
            }

            // Convert the values to the range of 0-255
            r = Math.round(r * 255);
            g = Math.round(g * 255);
            b = Math.round(b * 255);

            return { r: r, g: g, b: b };
        }
</script>
</html>
)rawliteral";