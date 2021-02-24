#include <Arduino.h>
#include <Joystick.h>
#include <SegaController.h>
#include "constants.h"
#include "settings.h"
#include "key_mode_default.h"
#include "led_control.h"

extern bool boot_done;

unsigned long pwr_timer;
int joykey_mode = KEY_MODE_DEFAULT;
bool swap_ports = false;
unsigned long key_debounce[PORT_COUNT][KEY_COUNT];
byte key_state[PORT_COUNT][KEY_COUNT];

Joystick_ Joystick[PORT_COUNT] = {
  Joystick_(
    0x03, 
    JOYSTICK_TYPE_GAMEPAD, 
    8, 0,                 // Button Count, Hat Switch Count
    true, true, false,    // X and Y, but no Z Axis
    false, false, false,  // No Rx, Ry, or Rz
    false, false,         // No rudder or throttle
    false, false, false   // No accelerator, brake, or steering
  ),
  Joystick_(
    0x04, 
    JOYSTICK_TYPE_GAMEPAD, 
    8, 0,                 // Button Count, Hat Switch Count
    true, true, false,    // X and Y, but no Z Axis
    false, false, false,  // No Rx, Ry, or Rz
    false, false,         // No rudder or throttle
    false, false, false   // No accelerator, brake, or steering
  )
};

// Specify the Arduino pins that are connected to
// DB9 Pin 7, DB9 Pin 1, DB9 Pin 2, DB9 Pin 3, DB9 Pin 4, DB9 Pin 6, DB9 Pin 9
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
  switch (mode) {
    case KEY_MODE_PADDLES:
      //init_mode_paddles();
      #ifndef FORCE_ALTERNATE
        delay(BOOT_DELAY);
      #endif
      break;
    
    case KEY_MODE_DEFAULT:
    default:
      init_mode_default();
      break;
  }
  return true;
}

void handle_mode() {
  switch (joykey_mode) {
    case KEY_MODE_PADDLES:
      //handle_mode_paddles();
      break;
    
    case KEY_MODE_DEFAULT:
    default:
      handle_mode_default();
      break;
  }
}

/* Manually clear key state. */
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
            swap_ports = !swap_ports;
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