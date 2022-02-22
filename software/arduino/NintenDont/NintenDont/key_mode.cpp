#include <Arduino.h>
#include <Joystick.h>
#include "constants.h"
#include "settings.h"
#include "key_mode_default.h"
#include "led_control.h"

extern bool boot_done;

unsigned long pwr_timer;
int joykey_mode = PRIMARY_MODE;

/* Uses the ArduinoJoystickLibrary by MHeironimus, this section is mostly based
 * on his Gamepad-example that comes with the library. The bugs are probably my
 * contribution to the greater good.
 * 
 * https://github.com/MHeironimus/ArduinoJoystickLibrary
 */

Joystick_ Joystick = Joystick_(
    0x03, 
    JOYSTICK_TYPE_GAMEPAD, 
    5, 0,                 // Button Count, Hat Switch Count
    true, true, false,    // X and Y, but no Z Axis
    false, false, false,  // No Rx, Ry, or Rz
    false, false,         // No rudder or throttle
    false, false, false   // No accelerator, brake, or steering
  );

bool init_mode(byte mode) {
  joykey_mode = mode;
  boot_done = true;

  /* Turn on LED if primary mode, flash on alternate mode instead. */
  if (mode == PRIMARY_MODE) set_pwr(true);
  else flash_pwr(3);

  switch (mode) {
    // case KEY_MODE_TURBO:
    //   mode_turbo::init_mode();
    //   #ifndef FORCE_ALTERNATE
    //     delay(BOOT_DELAY);
    //   #endif
    //   break;
    
    case KEY_MODE_DEFAULT:
    default:
      mode_default::init_mode();
      break;
  }
  return true;
}

void handle_mode() {
  switch (joykey_mode) {
    // case KEY_MODE_TURBO:
    //   mode_turbo::handle_mode();
    //   break;
    
    case KEY_MODE_DEFAULT:
    default:
      mode_default::handle_mode();
      break;
  }
}