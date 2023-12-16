
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

    .reinvented-color-wheel,.reinvented-color-wheel--hue-handle,.reinvented-color-wheel--hue-wheel,.reinvented-color-wheel--sv-handle,.reinvented-color-wheel--sv-space{touch-action:manipulation;touch-action:none;-webkit-touch-callout:none;-webkit-tap-highlight-color:transparent;-webkit-user-select:none;-moz-user-select:none;-ms-user-select:none;user-select:none}.reinvented-color-wheel{position:relative;display:inline-block;line-height:0;border-radius:50%}.reinvented-color-wheel--hue-wheel{border-radius:50%}.reinvented-color-wheel--sv-space{position:absolute;left:0;top:0;right:0;bottom:0;margin:auto}.reinvented-color-wheel--hue-handle,.reinvented-color-wheel--sv-handle{position:absolute;box-sizing:border-box;pointer-events:none;border-radius:50%;border:2px solid #fff;box-shadow:inset 0 0 0 1px #000}
  </style>
<link rel="stylesheet" href="/wheel.css">
<script src="/wheel.js"></script>
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

    picker = new ReinventedColorWheel({
                    appendTo: document.getElementById('color0'),
                    wheelDiameter: 250,
                    wheelThickness: 40,
                    handleDiameter: 35,
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

    UpdatePatterns()
    function UpdatePatterns() {
        fetch("/patterns").then((response) => {
            return response.json();
        }).then((animations) => {
            var animationHtml = ``;
            animations.forEach((animation) => {
                animationHtml += `<button onclick="sendAnimation(${animation.id}, ${animation.speed})">${animation.name}</button>`
            })
            document.getElementById("animations").innerHTML = animationHtml;
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


const char wheelCss[] PROGMEM = R"rawliteral(
.reinvented-color-wheel,.reinvented-color-wheel--hue-handle,.reinvented-color-wheel--hue-wheel,.reinvented-color-wheel--sv-handle,.reinvented-color-wheel--sv-space{touch-action:manipulation;touch-action:none;-webkit-touch-callout:none;-webkit-tap-highlight-color:transparent;-webkit-user-select:none;-moz-user-select:none;-ms-user-select:none;user-select:none}.reinvented-color-wheel{position:relative;display:inline-block;line-height:0;border-radius:50%}.reinvented-color-wheel--hue-wheel{border-radius:50%}.reinvented-color-wheel--sv-space{position:absolute;left:0;top:0;right:0;bottom:0;margin:auto}.reinvented-color-wheel--hue-handle,.reinvented-color-wheel--sv-handle{position:absolute;box-sizing:border-box;pointer-events:none;border-radius:50%;border:2px solid #fff;box-shadow:inset 0 0 0 1px #000}
)rawliteral";

const char wheelJs[] PROGMEM = R"rawliteral(
var ReinventedColorWheel=function(){"use strict"
var e=function(e){var t=e[0],n=e[1]/100,h=e[2]/100
return 0===h?[0,0,0]:[t,2*(n*=(h*=2)<=1?h:2-h)/(h+n)*100,(h+n)/2*100]}
var t,n,h,i=function(e){var t,n,h=e[0],i=e[1]/100,s=e[2]/100
return t=i*s,[h,100*(t=(t/=(n=(2-i)*s)<=1?n:2-n)||0),100*(n/=2)]},s="PointerEvent"in window
!s&&"ontouchend"in window?(t=function(e,t){return e.addEventListener("touchstart",function(n){(h=1===n.touches.length&&e)&&(n.preventDefault(),t(n.targetTouches[0]))})},n=function(e,t){return e.addEventListener("touchmove",function(n){h===e&&(n.preventDefault(),t(n.targetTouches[0]))})}):(t=function(e,t){return e.addEventListener(s?"pointerdown":"mousedown",function(n){0===n.button&&(h=e,t(n))})},n=function(e,t){return addEventListener(s?"pointermove":"mousemove",function(n){h===e&&t(n)})},addEventListener(s?"pointerup":"mouseup",function(){h=void 0}))
var o={hsv:[0,100,100],hsl:[0,100,50],wheelDiameter:200,wheelThickness:20,handleDiameter:16,wheelReflectsSaturation:!0,onChange:function(){}}
function l(e,t){return e?[r(e[0])?(n=e[0],h=360,i=(n+.5|0)%h,i<0?i+h:i):t[0],r(e[1])?a(e[1]):t[1],r(e[2])?a(e[2]):t[2]]:t
var n,h,i}function a(e){return e<0?0:e>100?100:e}function r(e){return"number"==typeof e&&isFinite(e)}function d(e,t){var n=document.createElement(e)
return n.className=t,n}return function(){function s(e){var i=this
this.options=e,this.wheelDiameter=this._option("wheelDiameter"),this.wheelThickness=this._option("wheelThickness"),this.handleDiameter=this._option("handleDiameter"),this.onChange=this._option("onChange"),this.wheelReflectsSaturation=this._option("wheelReflectsSaturation"),this.rootElement=this.options.appendTo.appendChild(d("div","reinvented-color-wheel")),this.hueWheelElement=this.rootElement.appendChild(d("canvas","reinvented-color-wheel--hue-wheel")),this.hueWheelContext=this.hueWheelElement.getContext("2d"),this.hueHandleElement=this.rootElement.appendChild(d("div","reinvented-color-wheel--hue-handle")),this.svSpaceElement=this.rootElement.appendChild(d("canvas","reinvented-color-wheel--sv-space")),this.svSpaceContext=this.svSpaceElement.getContext("2d"),this.svHandleElement=this.rootElement.appendChild(d("div","reinvented-color-wheel--sv-handle")),this._redrawHueWheel=function(){i._redrawHueWheelRequested=!1
var e=i.wheelDiameter,t=e/2,n=t-i.wheelThickness/2,h=Math.PI/180,s=i.wheelReflectsSaturation?","+i.hsl[1]+"%,"+i.hsl[2]+"%)":",100%,50%)",o=i.hueWheelContext
o.clearRect(0,0,e,e),o.lineWidth=i.wheelThickness
for(var l=0;l<360;l++)o.beginPath(),o.arc(t,t,n,(l-90.7)*h,(l-89.3)*h),o.strokeStyle="hsl("+l+s,o.stroke()},this._onMoveHueHandle=function(e){var t=i.hueWheelElement.getBoundingClientRect(),n=i.wheelDiameter/2,h=e.clientX-t.left-n,s=e.clientY-t.top-n,o=Math.atan2(s,h)
i.setHSV(180*o/Math.PI+90)},this._onMoveSvHandle=function(e){var t=i.svSpaceElement.getBoundingClientRect(),n=100*(e.clientX-t.left)/t.width,h=100*(t.bottom-e.clientY)/t.height
i.setHSV(i.hsv[0],n,h)},this.hueWheelContext.imageSmoothingEnabled=!1,this.svSpaceContext.imageSmoothingEnabled=!1,!e.hsv&&e.hsl?this.hsv=s.hsl2hsv(this.hsl=l(e.hsl,o.hsl)):this.hsl=s.hsv2hsl(this.hsv=l(e.hsv,o.hsv)),t(this.hueWheelElement,function(e){var t=i.hueWheelElement.getBoundingClientRect()
i.hueWheelContext.getImageData(e.clientX-t.left,e.clientY-t.top,1,1).data[3]?i._onMoveHueHandle(e):h=void 0}),n(this.hueWheelElement,this._onMoveHueHandle),t(this.svSpaceElement,this._onMoveSvHandle),n(this.svSpaceElement,this._onMoveSvHandle),this.redraw()}return s.prototype.setHSV=function(){var e=this.hsv,t=this.hsv=l(arguments,e),n=e[0]-t[0],h=e[1]-t[1]||e[2]-t[2]
n&&(this.hsl[0]=this.hsv[0],this._redrawHueHandle(),this._updateSvBackground()),h&&(this.hsl=s.hsv2hsl(t),this._redrawSvHandle(),this.wheelReflectsSaturation&&!this._redrawHueWheelRequested&&(requestAnimationFrame(this._redrawHueWheel),this._redrawHueWheelRequested=!0)),(n||h)&&this.onChange(this)},s.prototype.setHSL=function(){this.setHSV.apply(this,s.hsl2hsv(l(arguments,this.hsl)))},s.prototype.redraw=function(){this.hueWheelElement.width=this.hueWheelElement.height=this.wheelDiameter,this.svSpaceElement.width=this.svSpaceElement.height=(this.wheelDiameter-2*this.wheelThickness)*Math.sqrt(2)/2
var e=this.hueHandleElement.style,t=this.svHandleElement.style
e.width=e.height=t.width=t.height=this.handleDiameter+"px",e.marginLeft=e.marginTop=t.marginLeft=t.marginTop=-this.handleDiameter/2+"px",this._redrawHueWheel(),this._redrawHueHandle(),this._redrawSvSpace(),this._redrawSvHandle()},s.prototype._redrawSvSpace=function(){this._updateSvBackground()
var e=this.svSpaceElement.width,t=this.svSpaceContext,n=t.createLinearGradient(0,0,e,0),h=t.createLinearGradient(0,0,0,e)
n.addColorStop(0,"rgba(255,255,255,1)"),n.addColorStop(1,"rgba(255,255,255,0)"),h.addColorStop(0,"rgba(0,0,0,0)"),h.addColorStop(1,"rgba(0,0,0,1)"),t.fillStyle=n,t.fillRect(0,0,e,e),t.fillStyle=h,t.fillRect(0,0,e,e)},s.prototype._updateSvBackground=function(){this.svSpaceElement.style.backgroundColor="hsl("+this.hsv[0]+",100%,50%)"},s.prototype._redrawHueHandle=function(){var e=this.wheelDiameter/2,t=e-this.wheelThickness/2,n=(this.hsv[0]-90)*Math.PI/180,h=this.hueHandleElement.style
h.left=t*Math.cos(n)+e+"px",h.top=t*Math.sin(n)+e+"px"},s.prototype._redrawSvHandle=function(){var e=this.svSpaceElement,t=this.svHandleElement.style
t.left=e.offsetLeft+e.offsetWidth*this.hsv[1]/100+"px",t.top=e.offsetTop+e.offsetHeight*(1-this.hsv[2]/100)+"px"},s.prototype._option=function(e){var t=this.options[e]
return void 0!==t?t:o[e]},s.default=s,s.defaultOptions=o,s.hsv2hsl=i,s.hsl2hsv=e,s}()}())rawliteral";