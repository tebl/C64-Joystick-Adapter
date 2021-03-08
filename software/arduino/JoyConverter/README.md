# JoyConverter
This was the original firmware for the C64 Joystick Adapter, it allows you to use all of the original Commodore 64 controllers on a modern computer over USB (including most Raspberry Pi-based options). Out of the box it supports the additonal fire button on the joysticks that came with the Commodore 64GS console - although I shouldn't have to mention it, this means a controller wired for active high on this pin (anything else, like the Sega Mega Drive or Sega Master System gamepads would sooner or later kill the SID chip on an original system).

Emulators, like the original system, have the rather irritating problem that various games would either require a joystick in port 1 or port 2 - forcing you to move the cable all the time. For this reason, JoyConverter swaps the ports around when **MODE** on the device is pushed.

The firmware has an alternate mode that allows you to use paddles, this maps each of the two paddles to the X and Y directions. You can access the alternate mode by holding down **MODE** when plugging in the adapter, after around a second it will blink the LED to know that it is in this mode.

## Settings
The Arduino sketch have some configurable options, you'll need to edit the file named **settings.h**. Note that you'll have to recompile and upload the new version of the firmware every time you make changes to it. If you want the device to always be in the alternate mode, for use with paddles, you can enable **FORCE_ALTERNATE** (remove the two slashes in front of it).
```
//#define FORCE_PRIMARY
//#define FORCE_ALTERNATE
```