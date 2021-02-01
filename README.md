# C64 Joystick to USB
I picked up one of those fancy new Raspberry Pi 400 boxes lately, mainly in order to have just one more piece of technology in my home to play Commodore 64 games on - in addition to the actual Commodore 64 computers in almost all shapes and sizes. I could use my [C64 JoyKEY](https://github.com/tebl/C64-JoyKEY) in USB joystick mode on the new hardware, but that one is a whole different project - this is mostly focused around connecting one to two Atari-style joysticks to your modern computer over USB (note that modern in this case includes just about anything this side of the millennium).

![C64 Joystick Adapter](https://github.com/tebl/C64-Joystick-Adapter/raw/main/gallery/adapter.jpg)

It is only meant as a fun little project, but if you need one and would like to build it yourself - then this project contains PCB designs you need in addition to the Arduino code that powers it all. A picture of the completed unit along with one of the excellent ArcadeR joystick from RetroRadionics can be seen below.

![System picture](https://github.com/tebl/C64-Joystick-Adapter/raw/main/gallery/system.jpg)

# 1> Building the adapter
I'm not going to go into great detail on this point as there is not much that can go wrong when soldering together this unit, just ensure that you have tools of at least moderate quality and you should not have much problems with it. That usually means a temperature-controlled soldering iron, some solder that isn't the kind used to weld pipes (the electronics version) and a table that isn't considered flamable. First things first though, is actually getting the parts - check the [BOM](#2-bom)-section below for a complete list. 

Everything ready to go? Then start your soldering iron, wait until sufficiently hot without getting distracted by the TV and start with the smallest of the components - the resistors. Work your way up the size-scale from that point until you've populated everything, or at least populated the components you have an interest in having there. Save the Arduino Pro Micro for last.

Over to the Arduino Pro Micro, as advertised. At this point you'll need to consider if you want to be able to remove the board and use it for another project later, if so - use some female header pins on either the adapter PCB or the Arduino PCB so that you can unplug it later (desoldering is hard, but they're only around 5$ so might as well just buy a new one). When soldering on the pins, ensure that it fits into the form factor your board has (some are a bit smaller, which in not so many words, explains why there are two rows of pins on one side).

# 1.1> In case of trouble
There are not many components on this board, but if you get into trouble there is usually a way to work logically through the circuit in order to pinpoint where the fault is; is the Arduino working, or is there just one part of the circuit that does not appear to work? One problem might simply be that the Arduino is faulty -  they are produced at minimal cost, so it happens - try if a simple blink sketch work on it first. If you can't get anything to install on it, including the blink sketch - there is a good troubleshooting article on the [sparkfun](https://learn.sparkfun.com/tutorials/pro-micro--fio-v3-hookup-guide/all) (note that you'll most likely need the reset switch installed on the PCB).

For everything that has already been mentioned, and the things I've probably forgotten to include - you'll need to check out the [schematic](https://github.com/tebl/C64-Joystick-Adapter/tree/main/documentation/schematic) for this project.

# 1.2> Installing the firmware
Installing the firmware is mostly just opening up the Arduino sketch found within the software directory of this project, in case you're wondering - the reason for the elaborate folder structure is because I'm using PlatformIO as my development environment, but if you only want to use the device you'll only need *Arduino Studio*.

If you're using PlatformIO you probably won't need to perform any further steps to make it all work, for the Arduino Studio there is one separate library that you will need to install manually. The library is called [ArduinoJoystickLibrary](https://github.com/MHeironimus/ArduinoJoystickLibrary), you'll have to download and install that one according to the  [installation-instructions](https://github.com/MHeironimus/ArduinoJoystickLibrary#installation-instructions) provided by the author.

Finally you'll need to ensure that you have the correct board type selected for your Arduino Pro Micro. If you can't find the one shown in the image below, you might need to select something that is functionally the same or install the board definitions I used - instructions for doing this is available on the [sparkfun](https://learn.sparkfun.com/tutorials/pro-micro--fio-v3-hookup-guide/all)-article that was also mentioned in section [1.1](#11-in-case-of-trouble). Selecting the wrong one might make the board stop responding, fixing this is explained in the same article - usually by pressing the reset button twice and flashing the sketch to the device within 8 seconds (with the correct board type selected).

![Board selection](https://github.com/tebl/C64-Joystick-Adapter/raw/main/software/arduino/JoyConverter/arduino_settings.png)

# 2> BOM
If you want to build a C64 Joystick Adapter that converts standard Atari-style joysticks into USB joysticks, then this is mainly the list of all the parts that you'd need for it. Most you should be able to get from your friendly neighbourhood electronics store, but I don't have any of those - so consider this a list of search tips for ebay, AliExpress, banggood or any similar site. Any parts listed in parenthesis might strictly not be needed for basic functionality for a single joystick, but if you're ordering stuff you might as well build the whole thing.

| Reference            | Item                                              | Count | Order  |
| -------------------- | ------------------------------------------------- | ----- | ------ |
| Adapter (PCB)        | Fabricate using Gerber files                      |     1 | PCBWay |
| A1                   | Arduino Pro Micro                                 |     1 |        |
| D1                   | 5mm LED                                           |    (1)|        |
| J1,J2                | Female DB9 right-angle connector                  |  1 (1)|        |
| R1-R4                | 220k Ohm resistor                                 |    (4)|        |
| R5                   | 330 Ohm resistor                                  |    (1)|        |
| SW1,SW2              | 6x6x5mm right-angle momentary button              |    (2)|        |
