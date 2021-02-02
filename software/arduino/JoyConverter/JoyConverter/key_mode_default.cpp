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
extern unsigned long pwr_timer;
extern unsigned long key_debounce[PORT_COUNT][KEY_COUNT];
extern byte key_state[PORT_COUNT][KEY_COUNT];
extern Joystick_ Joystick[PORT_COUNT];
extern bool swap_ports;

void init_mode_default() {
  set_pwr(true);
  pwr_timer = millis() + LED_SHUTOFF;

  pinMode(PIN_MODE, INPUT_PULLUP);

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
  }

  update_joystick(port_id);
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