# The64Genesis

I picked up a The64 Mini on a whim and a sale, more than likely because someone in the shop had tried it out at some point and come to some conclusions regarding the quality of the thing. The machine itself is super-cute with its less than functional keyboard and breadbox-y shape, but the reviews really did not go far enough in explaining exactly how dreadful the controller really is. Commodore 64 games are hard enough, even with a sensible joystick so I needed something else.

I started out with the GenesisMachine firmware I made earlier, then discovered that none of the keys worked since the Mini have some sort of controller whitelist and well-behaved Arduino Pro Micro solutions simply arent't on the list - so I did the next best thing; copy the relevant bits from people who actually know what they are doing. In this case relief was found in the form of an [article](https://www.tsb.space/projects/c64-mini-arcade2usb-converter/) written by TSB, it links further to his [github](https://github.com/iamtsb/arcade2usb-converter)-repository.

# Controller layout
The resulting adapter have buttons laid out according to the illustration below, the yellow buttons are accessed by holding down the MODE-button that should be available on regular 6-button controllers. When activating any of the functions, be sure to hold down the MODE button as it detects a press by the release of a button - trying to push both at the same time will probably not perform as expected. Autofire can be enabled and disabled for the two fire buttons individually, a quick function by holding MODE and pushing B can be used to quickly disable both of them at the time (so that you don't have to remember or guess at which ones are currently active).

![Controller layout](https://github.com/tebl/C64-Joystick-Adapter/raw/main/software/arduino/The64Genesis/controller_layout.png)

Playing Commodore 64 games on a gamepad isn't always easy though, especially the precision needed in a lot of platformers and a few racing games will be harder to do perform on a gamepad. For this reason, most of the Mega Drive/Genesis adapters for the real Commodore 64 comes with the ability to use the C button instead of the UP button on the gamepad. Holding MODE and pushing MENU, alternatively pushing MODE on the C64 Joystick Adapter, enables this mode of operation. Note that this replaces the turbo functionality, D-pad UP will be deactivated to avoid accidental presses.

# Buiding The64Genesis
The instructions for building has been provided in the main project, for the most part this includes installing the controller ports and soldering the jumpers for Sega pinouts. The following sections mainly deal with installing the firmware on a suitable Arduino Pro Micro clone.

## Firmware installation
The firmware is supplied in the form of an Arduino-sketch, you'll find it under The64Genesis subfolder. The following subsections goes through setting up additional libraries needed, changing the Arduino board identifier needed and how to change any settings (should you want to do so).

### Installing libraries
Given that we need Arduino IDE to compile and upload the firmware, more on this later, we'll need to install an extra library. Thankfully I'm lazy when it comes to adopting new libraries, so there's only one that needs to be installed - [ArduinoJostickLibrary](https://github.com/MHeironimus/ArduinoJoystickLibrary). You can follow the documentation on the page for the exact details, but in short it is: download [library](https://github.com/MHeironimus/ArduinoJoystickLibrary/archive/master.zip) in zip-format, install it from the Arduino IDE  by going to *Tools --> Include library --> Add .ZIP Library*.

### Changing Arduino board identifier
As the already mentioned article describess, the solution works by changing the HID identifier for the Arduino Pro Micro so that it matches up to a different controller that is already in the mentioned whitelist. This is done by adding the contents of the supplied boards.txt file into the one used by the Arduino software, on my system the complete path to this file is *C:\Program Files (x86)\Arduino\hardware\arduino\avr\boards.txt* - you'll probably have to edit as an administrator. While the code was developed using PlatformIO, you'll need to compile and upload the code using Arduino IDE in order to make this work (select *C64 Joystick Adapter* from the list of boards instead of Arduino Leonardo/Arduino Pro Micro).

Open up the *The64Genesis.ino* sketch found in the folder with the same name, if all things are going according to plan - you'll see a new option in the list of boards (as shown below). Compile and upload as normal.

![Board setup](https://github.com/tebl/C64-Joystick-Adapter/raw/main/software/arduino/The64Genesis/arduino_settings.png)

### Settings
The Arduino sketch have some configurable options, you'll need to edit the file named **settings.h**. Have a read through the comments to see if there is anything you want to customize to your liking, lines without a value attached to them are disabled by commenting the line.

Given the example below, you would enable the autofire functionality by removing the two slashes (//) in front of *"#define ENABLE_AUTO_FIRE"*. The other values you just need to change the values, in this particular case - a lower value means a faster autofire or turbo fire (referred to as *rapid fire* here) response. Note that the software on the other end will have a hard time to follow if this is set too low, but feel free to experiment until you find what feels good for you and the games you play.
```
//#define ENABLE_AUTO_FIRE
#define AUTO_FIRE_PERIOD_ON 80
#define AUTO_FIRE_PERIOD_OFF 80
#define RAPID_FIRE_PERIOD_ON 40
#define RAPID_FIRE_PERIOD_OFF 40
```

**NB!** Note that you'll have to recompile and upload the new version of the firmware every time you make changes to it.

# Design caveats
I don't know of a way that the mini can support more than one controller this way, so with this firmware only Port 2 will be active. Comment out the SWAP_PORTS line in *settings.h* if you want to plug your Sega Mega Drive controller into port 1 instead. The firmware will function as normal with only a 3-button Sega Master System controller, but in order to navigate all the features of The64 Mini you'll want a 6-button controller.