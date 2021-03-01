#include <Arduino.h>
#include <Joystick.h>
#include <SegaController.h>
#include "constants.h"
#include "settings.h"
#include "led_control.h"
#include "key_mode.h"

extern unsigned long pwr_timer;
extern Joystick_ Joystick[PORT_COUNT];
extern bool swap_ports;
extern bool c_to_jump;
extern bool swap_abxy;

extern SegaController gamepad_1;
extern word gamepad_1_state;
extern word gamepad_1_last;
extern SegaController gamepad_2;
extern word gamepad_2_state;
extern word gamepad_2_last;

extern unsigned long autofire_timer[PORT_COUNT][KEY_COUNT];
extern bool autofire_enabled[PORT_COUNT][KEY_COUNT];
extern byte key_state[PORT_COUNT][KEY_COUNT];

namespace mode_default {
  void init_mode() {
    pwr_timer = millis() + LED_SHUTOFF;
    pinMode(PIN_MODE, INPUT_PULLUP);

    /* Initialize port */
    for (int index = 0; index < PORT_COUNT; index++) {
      Joystick[index].setXAxisRange(-1, 1);
      Joystick[index].setYAxisRange(-1, 1);
      Joystick[index].begin(false);
    }
  }

  void check_autofire_timers(const int port_id, const byte key_id) {
    /* Handle auto fire */
    if (autofire_enabled[port_id][key_id]) {
      if (millis() > autofire_timer[port_id][key_id]) {
        flip_state(port_id, key_id, AUTO_FIRE_PERIOD_ON, AUTO_FIRE_PERIOD_OFF);
      }
    } else {
      clear_state(port_id, key_id);
    }
  }

  /* Handle fire button, a normal press of the button will always take precedence
  * over auto fire. Autofire can be activated by holding MODE and then pushing
  * the button
  */
  void handle_autofire(const int port_id, const byte key_id, const word key_mask, const word gamepad_state, const word gamepad_last) {
    #ifdef ENABLE_AUTO_FIRE
      if (is_key_active(gamepad_state, SC_BTN_MODE)) {
        /* Toggle autofire mode at the release of a button, but only do so while
        * the MODE button is being held on the controller. */
        if (!is_key_active(gamepad_state, key_mask) && is_key_active(gamepad_last, key_mask)) {
          autofire_enabled[port_id][key_id] = !autofire_enabled[port_id][key_id];
        }

        /* Keep auto fire running */
        check_autofire_timers(port_id, key_id);
      } else {
        /* Process button presses normally, run autofire if not being held. */
        if (is_key_active(gamepad_state, key_mask)) {
          Joystick[port_id].setButton(key_id, is_key_active(gamepad_state, key_mask));
        } else {
          check_autofire_timers(port_id, key_id);
        }
      }
    #else
      Joystick[port_id].setButton(key_id, is_key_active(gamepad_state, key_mask));
    #endif
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

    /* Replace UP direction with the C button */
    if (c_to_jump) {
      if (is_key_active(gamepad_state, SC_BTN_C)) Joystick[port_id].setYAxis(-1);
      else if (is_key_active(gamepad_state, SC_BTN_DOWN)) Joystick[port_id].setYAxis(1);
      else Joystick[port_id].setYAxis(0);
    } else {
      if (is_key_active(port_id, SC_BTN_UP) && is_key_active(port_id, SC_BTN_DOWN)) Joystick[port_id].setYAxis(0);
      else if (is_key_active(gamepad_state, SC_BTN_UP)) Joystick[port_id].setYAxis(-1);
      else if (is_key_active(gamepad_state, SC_BTN_DOWN)) Joystick[port_id].setYAxis(1);
      else Joystick[port_id].setYAxis(0);
      handle_autofire(port_id, KEY_C, SC_BTN_C, gamepad_state, gamepad_last);
    }

    /* Swap A/B buttons around */
    if (swap_abxy) {
      handle_autofire(port_id, KEY_A, SC_BTN_B, gamepad_state, gamepad_last);
      handle_autofire(port_id, KEY_B, SC_BTN_A, gamepad_state, gamepad_last);
      handle_autofire(port_id, KEY_X, SC_BTN_Y, gamepad_state, gamepad_last);
      handle_autofire(port_id, KEY_Y, SC_BTN_X, gamepad_state, gamepad_last);
    } else {
      handle_autofire(port_id, KEY_A, SC_BTN_A, gamepad_state, gamepad_last);
      handle_autofire(port_id, KEY_B, SC_BTN_B, gamepad_state, gamepad_last);
      handle_autofire(port_id, KEY_X, SC_BTN_X, gamepad_state, gamepad_last);
      handle_autofire(port_id, KEY_Y, SC_BTN_Y, gamepad_state, gamepad_last);
    }
    handle_autofire(port_id, KEY_Z, SC_BTN_Z, gamepad_state, gamepad_last);
    handle_autofire(port_id, KEY_START, SC_BTN_START, gamepad_state, gamepad_last);
    #ifndef ENABLE_AUTO_FIRE
      Joystick[port_id].setButton(KEY_MODE, is_key_active(gamepad_state, SC_BTN_MODE));
    #endif
    Joystick[port_id].sendState();
  }

  /* Takes care of updating key states, update_joystick takes care of actually
  * sending the button states over USB.
  */
  void update_port(const int port_id) {
    switch (port_id) {
      case PORT_1:
        gamepad_1_state = gamepad_1.getState();
        update_joystick((swap_ports ? PORT_2 : PORT_1), gamepad_1_state, gamepad_1_last);
        gamepad_1_last = gamepad_1_state;

      case PORT_2:
      default:
        gamepad_2_state = gamepad_2.getState();
        update_joystick((swap_ports ? PORT_1 : PORT_2), gamepad_2_state, gamepad_2_last);
        gamepad_2_last = gamepad_2_state;
        break;
    }
  }

  void handle_mode() {
    if (millis() > pwr_timer) {
      #ifdef PWR_ACTIVITY_MIN
        fade_pwr(PWR_ACTIVITY_MIN);
      #else
        fade_pwr();
      #endif
      pwr_timer = millis() + LED_FADE_SPEED;
    }

    debounce_joystick_key(PORT_1, KEY_MODE, false);
    for (int port_id = 0; port_id < PORT_COUNT; port_id++) {
      update_port(port_id);
    }
  }
}