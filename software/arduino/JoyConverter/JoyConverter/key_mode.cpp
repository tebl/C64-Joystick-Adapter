#include <Arduino.h>
#include <Joystick.h>
#include "constants.h"
#include "settings.h"
#include "key_mode_default.h"
#include "key_mode_paddles.h"
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
    JOYSTICK_TYPE_JOYSTICK, 
    2, 0,                 // Button Count, Hat Switch Count
    true, true, false,    // X and Y, but no Z Axis
    false, false, false,  // No Rx, Ry, or Rz
    false, false,         // No rudder or throttle
    false, false, false   // No accelerator, brake, or steering
  ),
  Joystick_(
    0x04, 
    JOYSTICK_TYPE_JOYSTICK, 
    2, 0,                 // Button Count, Hat Switch Count
    true, true, false,    // X and Y, but no Z Axis
    false, false, false,  // No Rx, Ry, or Rz
    false, false,         // No rudder or throttle
    false, false, false   // No accelerator, brake, or steering
  )
};

bool init_mode(byte mode) {
  joykey_mode = mode;
  boot_done = true;
  switch (mode) {
    case KEY_MODE_PADDLES:
      init_mode_paddles();
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
      handle_mode_paddles();
      break;
    
    case KEY_MODE_DEFAULT:
    default:
      handle_mode_default();
      break;
  }
}

/* Check the state of the specified key_id, but note that we're not sending
 * anything to the computer at this point - this only updates the state
 * engine. Key presses won't be registered until a certain amount of time has
 * passed, this adds a miniscule amount of delay in order to debounce the keys.
 * The delay should be below a tenth of normal human reaction times, so it 
 * worth keeping in order to avoid jittery joystick responses.
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
    key_debounce[port_id][key_id] = 0;
    key_state[port_id][key_id] = KEY_STATE_NEUTRAL;
  }
}

/* Check if the specified key has been registered, and we're waiting for the
 * key to be released (this means debouncing has already been performed by
 * debounce_joystick_key).
 */
bool is_waiting_release(const int port_id, const byte key_id) {
  return key_state[port_id][key_id] == KEY_STATE_WAIT_RELEASE;
}