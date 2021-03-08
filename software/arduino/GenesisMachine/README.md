# GenesisMachine

This firmware was developed in order to use various Sega controllers with the *C64 Joystick Adapter* PCB, you can use the adapter with both 3 and 6 button *Sega Mega Drive* (*Sega Genesis* in North America) controllers as well as the Sega Master System gamepads. The differences between this and the Atari-style PCB is mainly that you do **not** install resistors R1-R4, the jumpers on the underside must also be set to the Sega position (pins 2-3). The firmware can be used with most modern systems, but note that if you want to use the adapter with a The64 Mini or Maxi you'll instead require the [The64Genesis](https://github.com/tebl/C64-Joystick-Adapter/tree/main/software/arduino/The64Genesis) firmware.

![](https://github.com/tebl/C64-Joystick-Adapter/raw/main/gallery/system2.jpg)
![Jumpers](https://github.com/tebl/C64-Joystick-Adapter/raw/main/gallery/build_003.jpg)

The usual mode of operation allows you to use all buttons on a 6-button controller, if the auto fire functionality has been enabled in the settings - then the controller *MODE* button will be used when toggling auto fire on/off (hold *MODE*, press the button to activate the function for and then release *MODE* in order to use this function reliably. The default controller layout is shown below.

![Default 6-button layout](https://github.com/tebl/C64-Joystick-Adapter/raw/main/software/arduino/GenesisMachine/controller_layout.png)

As with the original JoyConverter firmware for Atari-style joysticks, GenesisMachine also has an alternate mode of operation that is enabled by holding down *MODE* on the **device** (not the one on the controller). The LED will flash to show that you've successfully set it to the alternate mode. In the alternate mode, a 6-button controller will function as a 3-button turbo controller (XYZ is now used for the turbo function). As with the other firmware, you can also use autofire here though you can only enable it for the ABC buttons (again, hold *MODE* on the controller and press the button to toggle function on/off - then realease *MODE*). The alternate mode controller layout is shown below.

![3-button turbo layout](https://github.com/tebl/C64-Joystick-Adapter/raw/main/software/arduino/GenesisMachine/controller_alternate.png)

At this point you'll be wondering what the *MODE* button on the device does - well, it remaps the C button to the UP direction (disabling D-pad UP). This should make things a lot easier when using the adapter and controller combination on with games originally made with joystick in mind, such as platformers on C64 and Amiga that required you to press UP to jump). This is the same in either of the primary or alternate mode.

### Settings
The Arduino sketch have some configurable options, you'll need to edit the file named **settings.h**. Have a read through the comments to see if there is anything you want to customize to your liking, lines without a value attached to them are disabled by commenting the line.

Given the example below, you would enable the autofire functionality by removing the two slashes (//) in front of *"#define ENABLE_AUTO_FIRE"*. The other values you just need to change the values, in this particular case - a lower value means a faster autofire or turbo fire (referred to as *rapid fire* here) response. Note that the software on the other end will have a hard time to follow if this is set too low, but feel free to experiment until you find what feels good for you and the games you play.
```
//#define ENABLE_AUTO_FIRE
#define AUTO_FIRE_PERIOD_ON 80
#define AUTO_FIRE_PERIOD_OFF 80
#define RAPID_FIRE_PERIOD_ON 70
#define RAPID_FIRE_PERIOD_OFF 70
```

Note that you can configure the adapter to always be in the alternate mode, or just skip the button selection logic and always use the default layout. This behaviour can be selected by uncommenting either of the two following lines.

```
//#define FORCE_PRIMARY
//#define FORCE_ALTERNATE
```