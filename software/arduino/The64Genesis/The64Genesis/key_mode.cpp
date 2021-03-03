#include <Arduino.h>
#include <SegaController.h>
#include "joystick.h"
#include "constants.h"
#include "settings.h"
#include "key_mode_default.h"
//#include "key_mode_turbo.h"
#include "led_control.h"

extern bool boot_done;

unsigned long pwr_timer;
int joykey_mode = PRIMARY_MODE;
bool swap_ports = false;
bool c_to_jump = false;
bool swap_abxy = false;

unsigned long autofire_timer[PORT_COUNT][KEY_COUNT];
bool autofire_enabled[PORT_COUNT][KEY_COUNT];
unsigned long key_debounce[PORT_COUNT][KEY_COUNT];
byte key_state[PORT_COUNT][KEY_COUNT];

/* Sega gamepad states are read using the SegaController by Jon Thysell.
 *
 * https://github.com/jonthysell/SegaController
 */

SegaController gamepad_1(
  /* Sega DB9 pin 7 */ PIN_POT_AY,
  /*              1 */ PIN_P1_UP, 
  /*              2 */ PIN_P1_DOWN, 
  /*              3 */ PIN_P1_LEFT, 
  /*              4 */ PIN_P1_RIGHT, 
  /*              6 */ PIN_P1_FIRE, 
  /*              9 */ PIN_POT_AX
);
word gamepad_1_state = 0;
word gamepad_1_last = 0;

SegaController gamepad_2(
  /* Sega DB9 pin 7 */ PIN_POT_BY,
  /*              1 */ PIN_P2_UP, 
  /*              2 */ PIN_P2_DOWN, 
  /*              3 */ PIN_P2_LEFT, 
  /*              4 */ PIN_P2_RIGHT, 
  /*              6 */ PIN_P2_FIRE, 
  /*              9 */ PIN_POT_BX
);
word gamepad_2_state = 0;
word gamepad_2_last = 0;

bool init_mode(byte mode) {
  joykey_mode = mode;
  boot_done = true;

  #ifdef C_TO_JUMP
    c_to_jump = true;
  #endif
  #ifdef SWAP_PORTS
    swap_ports = true;
  #endif
  #ifdef SWAP_ABXY
    swap_abxy = true;
  #endif

  /* Turn on LED if primary mode, flash on alternate mode instead. */
  if (mode == PRIMARY_MODE) set_pwr(true);
  else flash_pwr(3);

  switch (mode) {
    case KEY_MODE_TURBO:
      //mode_turbo::init_mode();
      #ifndef FORCE_ALTERNATE
        delay(BOOT_DELAY);
      #endif
      break;
    
    case KEY_MODE_DEFAULT:
    default:
      mode_default::init_mode();
      break;
  }
  return true;
}

void handle_mode() {
  switch (joykey_mode) {
    case KEY_MODE_TURBO:
      //mode_turbo::handle_mode();
      break;
    
    case KEY_MODE_DEFAULT:
    default:
      mode_default::handle_mode();
      break;
  }
}

/* Handles the PWR led when it is under control of either autofire or rapid
 * fire. Which is a fancy way of saying that this determines if a minimum
 * power setting has been defined, if one has been - then we switch to that
 * instead of switching off.
 */
void set_linked_led(const bool value) {
  #ifdef LINK_AUTOFIRE_PWR
    if (value) {
      set_pwr(true);
    } else {
      #ifdef PWR_ACTIVITY_MIN
        set_pwr(PWR_ACTIVITY_MIN);
      #else
        set_pwr(false);
      #endif
    }
  #endif
}

/* Sets a specific button state and updates timers for the next transition,
 * this should only be used with the rapid fire and autofire modes.
 */
void set_state(const int port_id, const byte key_id, const uint16_t key_code, byte cycle, const int period) {
  set_linked_led(cycle == KEY_STATE_ON_CYCLE);
  if (cycle == KEY_STATE_ON_CYCLE) Joystick.button_press(key_code);
  autofire_timer[port_id][key_id] = millis() + period;
  key_state[port_id][key_id] = cycle;
}

void flip_state(const int port_id, const byte key_id, const uint16_t key_code, const int period_on, const int period_off) {
  if (key_state[port_id][key_id] == KEY_STATE_ON_CYCLE) set_state(port_id, key_id, key_code, KEY_STATE_OFF_CYCLE, period_off);
  else set_state(port_id, key_id, key_code, KEY_STATE_ON_CYCLE, period_on);
}

/* Clear key state, this is mainly just to ensure that we return to a neutral
 * state after turbo or autofire modes have previously been activated.
 */
void clear_state(const int port_id, const byte key_id) {
  if (key_state[port_id][key_id] != KEY_STATE_NEUTRAL) {
    set_linked_led(false);
    key_state[port_id][key_id] = KEY_STATE_NEUTRAL;
  }
  //Joystick[port_id].setButton(key_id, false);
}

/* Manually clear the debouce key state. */
void clear_key(const int port_id, const byte key_id) {
  key_debounce[port_id][key_id] = 0;
  key_state[port_id][key_id] = KEY_STATE_NEUTRAL;
}

/* This might seem ridiculously complicated for reading a single button, and
 * you're definitely right! Function was copied entirely from JoyConverter with
 * all its complexities, mainly to keep the mental mapping to a minimum when 
 * moving code between the projects.
 */
void debounce_joystick_key(const int port_id, const byte key_id, const bool invert) {
  int current_port = port_id;
  if (swap_ports) current_port = (port_id == PORT_1 ? PORT_2 : PORT_1);
  int active = (invert ? HIGH : LOW);

  if (digitalRead(KEY_PINS[current_port][key_id]) == active) {
    switch (key_state[port_id][key_id]) {
      case KEY_STATE_NEUTRAL:
        if (key_debounce[port_id][key_id] == 0) {
          key_debounce[port_id][key_id] = millis() + DEBOUNCE_DELAY;
          return;
        }

        if (millis() > key_debounce[port_id][key_id]) {
          key_state[port_id][key_id] = KEY_STATE_WAIT_RELEASE;

          if (port_id == PORT_1 && key_id == KEY_MODE) {
            flash_pwr(3);
            c_to_jump = !c_to_jump;
          } else {
            #ifdef PWR_ACTIVITY
              boost_pwr();
              pwr_timer = millis() + LED_FADE_SPEED;
            #endif
          }
          return;
        }
        break;
      
      case KEY_STATE_WAIT_RELEASE:
        /* Wait for a high state to release */
      default:
        break;
    }
  } else {
    clear_key(port_id, key_id);
  }
}

/* Check if the specified key has been registered, and we're waiting for the
 * key to be released (this means debouncing has already been performed by
 * debounce_joystick_key).
 */
bool is_waiting_release(const int port_id, const byte key_id) {
  return key_state[port_id][key_id] == KEY_STATE_WAIT_RELEASE;
}

/* Check if a key is currently active, this is mainly just masking out the
 * bits by using the bit mask supplied and returning whether we get a non-
 * zero result. Key masks are defined within the SegaController library.
 */
bool is_key_active(const word gamepad_state, const word key_mask) {
  return (gamepad_state & key_mask) > 0;
}