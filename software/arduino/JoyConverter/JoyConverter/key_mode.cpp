#include <Arduino.h>
#include "constants.h"
#include "settings.h"
#include "key_mode_default.h"

extern bool boot_done;
int joykey_mode = KEY_MODE_DEFAULT;

unsigned long key_debounce[PORT_COUNT][KEY_COUNT];
byte key_state[PORT_COUNT][KEY_COUNT];

bool is_waiting_release(const int port_id, const byte key_id) {
  return key_state[port_id][key_id] == KEY_STATE_WAIT_RELEASE;
}

bool init_mode(byte mode) {
  joykey_mode = mode;
  boot_done = true;
  switch (mode) {
    /*
    case KEY_MODE_AUTOFIRE:
      init_mode_autofire();
      #ifndef FORCE_ALTERNATE
        delay(BOOT_DELAY);
      #endif
      break;*/
    
    case KEY_MODE_DEFAULT:
    default:
      init_mode_default();
      break;
  }
  return true;
}

void handle_mode() {
  switch (joykey_mode) {
    /*
    case KEY_MODE_AUTOFIRE:
      handle_mode_autofire();
      break;
      */
    
    case KEY_MODE_DEFAULT:
    default:
      handle_mode_default();
      break;
  }
}