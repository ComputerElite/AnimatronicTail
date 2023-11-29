
#include <Arduino.h>
#include "params.h"

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
</body>
<script>
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

    function sendAnimation(animation, speed) {
        fetch(`/animation`, {
            method: "POST",
            body: animation
        })
        fetch(`/speed`, {
            method: "POST",
            body: speed
        })
    }
</script>
</html>
)rawliteral";