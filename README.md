# LocoBuffer-Nano

LocoBuffer-Nano is a Locobuffer based on an Arduino-Uno with the Arduino-Sketch LocoLinx.ino.
LocoLinx.ino is taken from https://github.com/mrrwa/LocoNet/blob/master/examples/LocoLinx/LocoLinx.ino and slightly modified:
- TXPin changed
- Baudrate selection added

The circuit diagram is based on a circuit board that I have in my stash.
I also added opto-isolation to separate the potentials between the LocoNet and a PC.

Important:
My Arduino-Nano uses CH340 as USB-Interface. I added a connection between Pin 9 of CH340 and D9 of Arduino-Nano to control CTS-Signal.
