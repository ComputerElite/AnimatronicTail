# Animatronic Tail by ComputerElte
This is a project for an animatronic tail with LEDs

# Contact
`computerelite` on Discord or open an issue on this repo. If you know me personal message me on Telegram.

# Project roadmap (personal tail)
* [ ] Replace 30led/m strip with 60 or 144led/m strip
* [ ] Add fur covering (which still lets light through)

# Videos and Pictures
ToDo 

# Building the tail
*As of 2022/12/16*
## Background
I saw animatronic tails by the tail company and said "yeah I can do that too".

## Parts
Sourced from Ebay, Hornbach, local shops and my local makerspace
#### Required
- 3d printer with enough PLA (around 500g should be sufficient)
- High torque servos (I used 30kg cm servos. I ordered 270° but 180° arrived which is still enough https://www.ebay.de/itm/295910063432)
- About 80-90cm of 6mm wire rope https://www.hornbach.de/p/drahtseil-poesamo-o-6-mm-stahl-verzinkt-meterware/5002820/
- ESP 8266 (you could also use an ESP 32 if you want to add more stuff)
- Fishing wire (used to actuate the tail. Thin Wire ~1.5mm-3mm may be a better choice here)
- Tape
- M3 screws in various lengths, washers, nuts and [threads](https://www.amazon.de/Gewindeeinsatz-Einpressmutter-Gewindebuchsen-Kunststoff-Box-Kunststoffteiledurch/dp/B09B7F2XM3/)

#### Optional
- USB C port for power https://www.ebay.de/itm/284920406558
- 1m WS2812 LED strip for lights! Preferrably 60led/m or more https://www.ebay.de/itm/374479289527

## Printing required parts
### Info
In a lot of finished prints I had to widen holes, add extra holes or hone down openings

### Segments for the tail
You can find files for the tail segments in stl/TailSegments. The file name is the diameter. Files ending in `_mounting` have mounting holes for screwing them to the case.

*If you need the CAD files ask me with a preferred format and I'll put them into the repo if possible.*

You can print as many as you want, usually in descending size but you can choose whatever you want. e. g. I printed 100m, 100mm, 90mm, 80mm, 70mm, 60mm, 50mm iirc.
After printing you'll have to [put M3 threads into the holes](https://www.youtube.com/shorts/iw2qNMok5ok). The big hole in the middle is for the wire rope, the small one at the side is for the M3 thread.
After inserting the thread you can secure the Tail segment.

### Case
The case can be printed as is. It has holes for the servos, the tail segment mount and, cable management and a belt.

### Belt holder extender (Optional)
This is an extension for the backplate of the case so it doesn't bend down as much. I experienced that the servo wheels slid on my ass so I made that extension.
Print `BeltHolderHolder.stl` once and `BeltHolderHolderHolder.stl` twice.

### Servo wheels
There are wheels which attach to the servos. The wire for pulling on the tile will be fastened to them. They are tailored to the servos I ordered.
One M3 screw simultaniously attaches the wheel to the servo and holds the wire for pulling the tail.

## Electronics
Electronics were soldered by hand with a hole plate.

### ESP8266
Create a board which has power in and ground. That is connected to the ESP VIN and GND pin. The servos and leds are also connected with the Power in as well as Data pins on the ESP.
I used header pins for easily connecting and disconnecting the parts.

## Assembly
### Assembling the tail part
Take your printed tail segments and screw them to the wire rope in roughly same spacing

### Assembling the case
Assembling the case is the hardest part as everything is hard to reach

1. Secure the tail part to the mounting holes with 4 screws, washers and nuts (secure everything like this unless explicitly stated otherwise or your know better).
2. Secure the belt holder extender by clamping the BeltHolderHolder to the back of the case with both BeltHolderHolderHolders. Simply use M3 screws and nuts to tighten everything equally all the way.
3. Secure the servos with 4 screws each. They should be inserted from the inside.
4. Connect all the electronics and power it on. The tail should return to its default position.
5. Secure the servo arms to the servo pointing to the top.
6. Screw the servo wheel in but not all the way.
7. Thread the wire through all holes in the tail segments then through the top hole of the servo wheel. Wrap it around the screw in the middle a few times and then tighten the screw holding the servo wheel on the servo.
   This will securely hold the wire in (more or less).
8. Bend the tail slightly upward while on your table. Then secure the wire with a triple knot (or more) on the other end of the tail. Both sides should have the same tension.
   Tension should be the highest when it's laying flat on your table.
9. Secure all electronics with tape and manage your cables.

I am aware that this isn't a complete guide. I will expand it if questions arise. However by modifying the code in some places you should have a working tail now.

