#include <Arduino.h>
#include <Joystick.h>
#include "constants.h"
#include "settings.h"
#include "led_control.h"
#include "key_mode.h"

/* Uses the ArduinoJoystickLibrary by MHeironimus, this section is mostly based
 * on his Gamepad-example that comes with the library. The bugs are probably my
 * contribution to the greater good.
 * 
 * https://github.com/MHeironimus/ArduinoJoystickLibrary
 */
unsigned long pwr_timer;
extern unsigned long key_debounce[PORT_COUNT][KEY_COUNT];
extern byte key_state[PORT_COUNT][KEY_COUNT];

Joystick_ Joystick[PORT_COUNT] = {
  Joystick_(
    0x03, 
    JOYSTICK_TYPE_GAMEPAD, 
    1, 0,                 // Button Count, Hat Switch Count
    true, true, false,    // X and Y, but no Z Axis
    false, false, false,  // No Rx, Ry, or Rz
    false, false,         // No rudder or throttle
    false, false, false   // No accelerator, brake, or steering
  ),
  Joystick_(
    0x04, 
    JOYSTICK_TYPE_GAMEPAD, 
    1, 0,                 // Button Count, Hat Switch Count
    true, true, false,    // X and Y, but no Z Axis
    false, false, false,  // No Rx, Ry, or Rz
    false, false,         // No rudder or throttle
    false, false, false   // No accelerator, brake, or steering
  )
};

void init_mode_default() {
  flash_pwr(3);
  set_pwr(true);
  pwr_timer = millis() + LED_SHUTOFF;

  pinMode(PIN_P1_UP, INPUT_PULLUP);
  pinMode(PIN_P1_DOWN, INPUT_PULLUP);
  pinMode(PIN_P1_LEFT, INPUT_PULLUP);
  pinMode(PIN_P1_RIGHT, INPUT_PULLUP);
  pinMode(PIN_P1_FIRE, INPUT_PULLUP);

  pinMode(PIN_P2_UP, INPUT_PULLUP);
  pinMode(PIN_P2_DOWN, INPUT_PULLUP);
  pinMode(PIN_P2_LEFT, INPUT_PULLUP);
  pinMode(PIN_P2_RIGHT, INPUT_PULLUP);
  pinMode(PIN_P2_FIRE, INPUT_PULLUP);

  /* Initialize port */
  for (int index = 0; index < PORT_COUNT; index++) {
    Joystick[index].setXAxisRange(-1, 1);
    Joystick[index].setYAxisRange(-1, 1);
    Joystick[index].begin(false);
  }
}

/* Check the state of the specified key_id, but note that we're not sending
 * anything to the computer at this point - this only updates the state
 * engine. Key presses won't be registered until a certain amount of time has
 * passed, this adds a miniscule amount of delay in order to debounce the keys.
 * The delay should be below a tenth of normal human reaction times, so it 
 * worth keeping in order to avoid jittery joystick responses.
 */
void debounce_joystick_key(const int port_id, const byte key_id) {
  if (digitalRead(KEY_PINS[port_id][key_id]) == LOW) {
    switch (key_state[port_id][key_id]) {
      case KEY_STATE_NEUTRAL:
        if(key_debounce[port_id][key_id] == 0) {
          key_debounce[port_id][key_id] = millis() + DEBOUNCE_DELAY;
          return;
        }

        if (millis() > key_debounce[port_id][key_id]) {
          key_state[port_id][key_id] = KEY_STATE_WAIT_RELEASE;
          #ifdef PWR_ACTIVITY
          boost_pwr();
          pwr_timer = millis() + LED_FADE_SPEED;
          #endif
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

/* Update and send the joystick state to the computer. Given that some
 * controllers ordinarily does not allow two opposing directions to be pressed
 * at once, we'll explicitly filter those states (read as neutral instead of
 * favoring a direction).
 */
void update_joystick(const int port_id) {
  if (is_waiting_release(port_id, KEY_LEFT) && is_waiting_release(port_id, KEY_RIGHT)) Joystick[port_id].setXAxis(0);
  else if (is_waiting_release(port_id, KEY_LEFT)) Joystick[port_id].setXAxis(-1);
  else if (is_waiting_release(port_id, KEY_RIGHT)) Joystick[port_id].setXAxis(1);
  else Joystick[port_id].setXAxis(0);

  if (is_waiting_release(port_id, KEY_UP) && is_waiting_release(port_id, KEY_DOWN)) Joystick[port_id].setYAxis(0);
  else if (is_waiting_release(port_id, KEY_UP)) Joystick[port_id].setYAxis(-1);
  else if (is_waiting_release(port_id, KEY_DOWN)) Joystick[port_id].setYAxis(1);
  else Joystick[port_id].setYAxis(0);

  Joystick[port_id].setButton(0, is_waiting_release(port_id, KEY_FIRE));
  Joystick[port_id].sendState();
}

/* Takes care of updating key states, effectively debouncing the keys to avoid
 * jitter and then make sure updates are sent to the computer.
 */
void update_port(const int port_id) {
  for (int key_id = 0; key_id < KEY_COUNT; key_id++) {
    debounce_joystick_key(port_id, key_id);
    update_joystick(port_id);
  }
}

void handle_mode_default() {
  if (millis() > pwr_timer) {
    #ifdef PWR_ACTIVITY_MIN
      fade_pwr(PWR_ACTIVITY_MIN);
    #else
      fade_pwr();
    #endif
    pwr_timer = millis() + LED_FADE_SPEED;
  }

  for (int port_id = 0; port_id < PORT_COUNT; port_id++) {
    update_port(port_id);
  }
}