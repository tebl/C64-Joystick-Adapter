#include <Arduino.h>
#include "constants.h"
#include "settings.h"
#include "key_mode_default.h"
#include "led_control.h"

extern bool boot_done;

unsigned long pwr_timer;
int joykey_mode = PRIMARY_MODE;

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