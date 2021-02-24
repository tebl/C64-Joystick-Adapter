#include <Arduino.h>
#include <Joystick.h>
#include <SegaController.h>
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
extern Joystick_ Joystick[PORT_COUNT];
extern bool swap_ports;

/* Sega gamepad states are read using the SegaController by Jon Thysell.
 *
 * https://github.com/jonthysell/SegaController
 */

extern SegaController gamepad_1;
extern word gamepad_1_state;
extern word gamepad_1_last;
extern SegaController gamepad_2;
extern word gamepad_2_state;
extern word gamepad_2_last;

void init_mode_default() {
  set_pwr(true);
  pwr_timer = millis() + LED_SHUTOFF;
  pinMode(PIN_MODE, INPUT_PULLUP);

  /* Initialize port */
  for (int index = 0; index < PORT_COUNT; index++) {
    Joystick[index].setXAxisRange(-1, 1);
    Joystick[index].setYAxisRange(-1, 1);
    Joystick[index].begin(false);
  }
}

bool is_key_active(const word gamepad_state, const word key_mask) {
  return (gamepad_state & key_mask) > 0;
}

/* Update and send the joystick state to the computer. */
void update_joystick(const int port_id, const word gamepad_state, const word gamepad_last) {
  if (gamepad_state != gamepad_last) {
    #ifdef PWR_ACTIVITY
      boost_pwr();
      pwr_timer = millis() + LED_FADE_DELAY;
    #endif
  }

  if (is_key_active(gamepad_state, SC_BTN_LEFT) && is_key_active(gamepad_state, SC_BTN_RIGHT)) Joystick[port_id].setXAxis(0);
  else if (is_key_active(gamepad_state, SC_BTN_LEFT)) Joystick[port_id].setXAxis(-1);
  else if (is_key_active(gamepad_state, SC_BTN_RIGHT)) Joystick[port_id].setXAxis(1);
  else Joystick[port_id].setXAxis(0);

  if (is_key_active(port_id, SC_BTN_UP) && is_key_active(port_id, SC_BTN_DOWN)) Joystick[port_id].setYAxis(0);
  else if (is_key_active(gamepad_state, SC_BTN_UP)) Joystick[port_id].setYAxis(-1);
  else if (is_key_active(gamepad_state, SC_BTN_DOWN)) Joystick[port_id].setYAxis(1);
  else Joystick[port_id].setYAxis(0);

  Joystick[port_id].setButton(0, is_key_active(gamepad_state, SC_BTN_A));
  Joystick[port_id].setButton(1, is_key_active(gamepad_state, SC_BTN_B));
  Joystick[port_id].setButton(2, is_key_active(gamepad_state, SC_BTN_C));
  Joystick[port_id].setButton(3, is_key_active(gamepad_state, SC_BTN_X));
  Joystick[port_id].setButton(4, is_key_active(gamepad_state, SC_BTN_Y));
  Joystick[port_id].setButton(5, is_key_active(gamepad_state, SC_BTN_Z));
  Joystick[port_id].setButton(6, is_key_active(gamepad_state, SC_BTN_START));
  Joystick[port_id].setButton(7, is_key_active(gamepad_state, SC_BTN_MODE));

  Joystick[port_id].sendState();
}

/* Takes care of updating key states, effectively debouncing the keys to avoid
 * jitter and then make sure updates are sent to the computer.
 */
void update_port(const int port_id) {
  switch (port_id) {
    case PORT_1:
      gamepad_1_state = gamepad_1.getState();
      update_joystick(PORT_1, gamepad_1_state, gamepad_1_last);
      gamepad_1_last = gamepad_1_state;

    case PORT_2:
    default:
      gamepad_2_state = gamepad_2.getState();
      update_joystick(PORT_2, gamepad_2_state, gamepad_2_last);
      gamepad_2_last = gamepad_2_state;
      break;
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