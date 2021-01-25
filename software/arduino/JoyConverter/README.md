# C64 Joystick Adapter
...

## Settings
The Arduino sketch have some configurable options, you'll need to edit the file named **settings.h**. Note that you'll have to recompile and upload the new version of the firmware every time you make changes to it.

If you want to use the device as a vintage controller with rapidfire/autofire without having to deal with holding whole failsafe situation, you can force it into this mode by using **FORCE_ALTERNATE** (remove the two slashes in front of it).
```
#define FORCE_ALTERNATE
```