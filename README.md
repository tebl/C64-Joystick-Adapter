# C64 Joystick to USB

As with around 90% of my projects, it all somehow leads back to the Commodore 64 though sometimes even going quite beyond them as well (see [build options](#1-build-options) for these). In this case the idea was to put together a simple adapter so that I could use my Atari-style joysticks, vintage or otherwise, with modern systems over USB (including Raspberry Pi, The64 and the regular ones such as MS Windows 10).

![System picture](https://github.com/tebl/C64-Joystick-Adapter/raw/main/gallery/system.jpg)

The [JoyConverter](https://github.com/tebl/C64-Joystick-Adapter/tree/main/software/arduino/JoyConverter) firmware also supports paddles, these are mapped to the X and Y position in addition to registering a fire button for each of them (this is because a single port can hold two paddles, so with two ports we could potentially have up to four paddles). This is the mode mentioned in **settings.h**, you can enter paddle mode by holding down the mode button when powering on the unit (hold it down until the LED blinks).

One rather irritating problem with the C64 was that various games expected that your joystick is inserted into different joystick ports. For that reason I've added a bit of logic to the firmware to solve this, pushing the **mode** button will swap the active port so that you don't need to swap the cables or deal with this in the emulator settings separately. It was irritating enough on the physical machine as well, so that bit can certainly be said to have been perfectly emulated as well (on a physical Commodore 64, a different solution is the [C64 Joystick Switcher](https://github.com/tebl/C64-Joystick-Switcher)).

# 1> Build options
As already hinted at, the PCB design has been designed so that you have a few alternatives as to how you want to build up the PCB as well as the firmware options that are available to you. Given that there are some differences in the components used, make sure that you have a read through of the following sections before commiting to either of them. Note that on the The64 you will only have access to one of the ports, but at least you won't have to use the original joystick.

# 1.1> C64 Joystick Adapter
This is the version of the adapter that has been described so far, it requires **all** of the components listed in the [BOM](#-bom) and allows you to use up to two Atari-style joysticks or paddles on a modern system with USB-adapters. On the back of the PCB there are a four solder jumpers that need to be soldered so that pins **1-2** are bridged (marked Atari) - pin 1 is indicated by a square pad.

This build supports the following firmware alternatives:

- [JoyConverter](https://github.com/tebl/C64-Joystick-Adapter/tree/main/software/arduino/JoyConverter)

# 1.2> Sega Gamepad Adapter
Uses the same PCB as the *C64 Joystick Adapter*, but instead of allowing you to plug in Atari-style joysticks - this version instead allows you to use Sega Master System and Sega Mega Drive (Sega Genesis in North America) controllers. It is constructed in mainly the same way, except that you should **not** install resistors R1-R4 and then link the four solder jumpers across pins **2-3** (marked Sega) - pin 1 is indicated by a square pad.

This build supports the follwing firmware alternatives:

- [GenesisMachine](https://github.com/tebl/C64-Joystick-Adapter/tree/main/software/arduino/GenesisMachine)
- [The64Genesis](https://github.com/tebl/C64-Joystick-Adapter/tree/main/software/arduino/The64Genesis)

# 2> Building the adapter
I'm not going to go into great detail on this point as there is not much that can go wrong when soldering together this unit, just ensure that you have tools of at least moderate quality and you should not have much problems with it. That usually means a temperature-controlled soldering iron, some solder that isn't the kind used to weld pipes (the electronics version) and a table that isn't considered flammable. First things first though, is actually getting the parts - check the [BOM](#3-bom)-section below for a complete list as to what goes where.

![Build 001](https://github.com/tebl/C64-Joystick-Adapter/raw/main/gallery/build_001.jpg)

Note that [build options](#1-build-options) describes different ways of assembling the board, the pictures in this section have been taken when building a Sega Gamepad adapter - for this reason the jumpers are shown with this taken into account.

Everything ready to go? Then start your soldering iron, wait until sufficiently hot without getting distracted by the TV and start with the smallest of the components - the resistors (R1-R4 not installed with Sega controllers). Work your way up the size-scale from that point until you've populated everything, or at least populated the components you have an interest in having there (depending on build options).

![Build 002](https://github.com/tebl/C64-Joystick-Adapter/raw/main/gallery/build_002.jpg)

The last bit to solder in is usually the Arduino Pro Micro, you'll note that one side of the PCB footprint will have two sets of pins next to each other - this is to account for the two most common size variations of the Arduino Pro being sold. Solder the jumpers on the underside according to the build options you want.

![Build 003](https://github.com/tebl/C64-Joystick-Adapter/raw/main/gallery/build_003.jpg)

The last finishing touches is adding the faceplate, this has been made so that it is reversible and you'll show the side that matches the build. 8mm nylon standoffs hex standoffs are mounted on the device using the M3 nylon nuts on the underside (snip off the parts of the screw that are too long). Screw the faceplate into place using the screws from the front.

![Build 004](https://github.com/tebl/C64-Joystick-Adapter/raw/main/gallery/system2.jpg)

# 2.1> Installing the firmware
Installing the firmware is mostly just opening up the Arduino sketch found within the software directory of this project, in case you're wondering - the reason for the elaborate folder structure is because I'm using PlatformIO as my development environment. It is recommended to have a read-through of the [build options](#1-build-options) in order to determine which firmware you are going to use with the device, in most cases however it'll be easiest to use *Arduino Studio* when compiling and flashing the sketch onto the Arduino (the The64 firmware selections also requires some modifications, see the documentation for those separately).

For the most part there are only two separate libraries that you will need to install manually. The library is called [ArduinoJoystickLibrary](https://github.com/MHeironimus/ArduinoJoystickLibrary), you'll have to download and install that one according to the  [installation-instructions](https://github.com/MHeironimus/ArduinoJoystickLibrary#installation-instructions) provided by the author. Essentially this means downloading the zip-file, then installing it using *Sketch --> Include library --> Add .ZIP library*. The second library is installed the same way, it is the [SegaController](https://github.com/jonthysell/SegaController) by Jon Thysell.

Finally you'll need to ensure that you have the correct board type selected for your Arduino Pro Micro. If you can't find the one shown in the image below, you might need to select something that is functionally the same or install the board definitions I used - instructions for doing this is available on the [sparkfun](https://learn.sparkfun.com/tutorials/pro-micro--fio-v3-hookup-guide/all)-article that was also mentioned in section [2.3](#23-in-case-of-trouble). Selecting the wrong one might make the board stop responding, fixing this is explained in the same article - usually by pressing the reset button twice and flashing the sketch to the device within 8 seconds (with the correct board type selected).

![Board selection](https://github.com/tebl/C64-Joystick-Adapter/raw/main/software/arduino/JoyConverter/arduino_settings.png)

**NB!** The The64 Mini/Maxi requires that a different boards definition is used, the addition of this is described in details in the documentation for the [firmware](https://github.com/tebl/C64-Joystick-Adapter/tree/main/software/arduino/The64Genesis). With the relevant section added to your *boards.txt*, the board selection should look like what is shown below. This is not required when using the adapter with other systems such as MS Windows, Raspberry Pi-based systems etc.

![The64 board selection](https://github.com/tebl/C64-Joystick-Adapter/raw/main/software/arduino/The64Genesis/arduino_settings.png)

# 2.2> Testing it out
Actually testing it out in practice depends on the operating system running on the machine you're using it on, but if it's anything other than MS Windows 10 or Mac OS I'll just assume that you know what you're doing. Can't help you with the Mac OS unless someone gifts me a brand new Mac for this purpose, so for now - I'll just describe things from a Windows-based perspective.

The operating system (*Windows* was the assumption if you're still here) will see the adapter as two separate joysticks, no matter if anything is plugged into them - this is just the way they work. For an overview just hit the Windows-button (still *Windows*, unfortunately) and start typing in something like *Set up USB game controllers* and start that - you should see both of them, double-click one and you'll be able to test it all out (four directions, one button).

When it comes to emulators I've tried out Vice for Windows, here you'll need to select and assign each joystick to a port - save settings and that should be it. Remember the irritation that came with the Commodore 64 the joystick to be in different ports with different games, it's still there so keep that in mind.

I also tried out the WinAPE emulator (emulate the Amstrad CPC line of computers), and as far as I can tell it works right out of the box. Yay!

**NB!** It's already been mentioned, but check out the details for the firmware you are using. Most of these have an alternative mode, this is accessed by holding down the *MODE* button when plugging in the controller - realease the button when you see a flashing to confirm that it is using the alternative mode. When using [JoyConverter](https://github.com/tebl/C64-Joystick-Adapter/tree/main/software/arduino/JoyConverter) this allows you to use paddles, in [GenesisMachine](https://github.com/tebl/C64-Joystick-Adapter/tree/main/software/arduino/GenesisMachine) this instead converts a 6-button Sega controller into a 3-button Turbo controller.

# 2.3> In case of trouble
There are not many components on this board, but if you get into trouble there is usually a way to work logically through the circuit in order to pinpoint where the fault is; is the Arduino working, or is there just one part of the circuit that does not appear to work?

One problem might simply be that the Arduino is faulty -  they are produced at minimal cost, so it happens - try if a simple blink sketch work on it first. If you can't get anything to install on it, including the blink sketch - there's a pretty good troubleshooting article on the [sparkfun](https://learn.sparkfun.com/tutorials/pro-micro--fio-v3-hookup-guide/all) (note that you'll most likely need the reset switch installed on the PCB).

For everything that has already been mentioned, and the things I've probably forgotten to include - you'll need to check out the [schematic](https://github.com/tebl/C64-Joystick-Adapter/tree/main/documentation/schematic) for this project.

# 3> BOM
If you want to build a C64 Joystick Adapter that converts standard Atari-style joysticks into USB joysticks, then this is mainly the list of all the parts that you'd need for it. Most you should be able to get from your friendly neighbourhood electronics store, but I don't have any of those - so consider this a list of search tips for ebay, AliExpress, banggood or any similar site. Any parts listed in parenthesis might strictly not be needed for basic functionality for a single joystick, but if you're ordering stuff you might as well just build the whole thing.

| Reference            | Item                                              | Count | Order  |
| -------------------- | ------------------------------------------------- | ----- | ------ |
| Adapter (PCB)        | Fabricate using Gerber files                      |     1 | [PCBWay](https://www.pcbway.com/project/shareproject/C64_Joystick_Adapter.html) |
| Faceplate (PCB) **   | Fabricate using Gerber files                      |    (1)| [PCBWay](https://www.pcbway.com/project/shareproject/C64_Joystick_Adapter__Faceplate_.html) |
| A1                   | Arduino Pro Micro                                 |     1 |        |
| D1                   | 5mm LED                                           |    (1)|        |
| J1,J2                | Male DB9 right-angle connector                  |  1 (1)|        |
| R1-R4                | 220k Ohm resistor                                 |    (4)|        |
| R5                   | 330 Ohm resistor                                  |    (1)|        |
| SW1 *                | 6x6mm momentary button                            |    (1)|        |
| SW2                  | 6x6x9mm momentary button                          |    (1)|        |
| **                   | M3 nylon nut                                      |    (4)|        |
| **                   | M3x8mm nylon hex standoff                         |    (4)|        |
| **                   | M3x6mm nylon screw                                |    (4)|        |

*) If you don't encounter any problems flashing the firmware onto the Arduino Pro Micro, I recommend not soldering in the Arduino *reset* button so you're not accidentally pressing it when frantically looking for the mode button instead.

**) These components are for the additional faceplate. Strictly not necessary, but keeps the  whole setup looking as stylish as it can be. The faceplate is reversible, so install it in the orientation to reflect the PCB build (Atari joystick or Sega gamepad).
