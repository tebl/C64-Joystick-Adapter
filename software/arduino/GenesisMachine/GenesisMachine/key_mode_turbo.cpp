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

namespace mode_turbo {
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

  /* Handle fire button pair, ABC should be processed normally and take precedence
  * while XYZ will serve as rapid fire buttons instead. Autofire can be activated
  * by holding MODE and then pushing the button, when active it will only cycle 
  * if none of the other modes are currently active.
  */
  void handle_turbo(const int port_id, const byte key_id, const word key_mask, const byte key_id2, const word key_mask2, const word gamepad_state, const word gamepad_last) {
    if (is_key_active(gamepad_state, key_mask)) {
      /* Handle regular button */
      Joystick[port_id].setButton(key_id, true);
    } else {
      /* Handle turbo button */
      if (is_key_active(gamepad_state, key_mask2)) {
        switch (key_state[port_id][key_id]) {
          case KEY_STATE_NEUTRAL:
            set_state(port_id, key_id, KEY_STATE_ON_CYCLE, RAPID_FIRE_PERIOD_ON);
            break;
          
          case KEY_STATE_ON_CYCLE:
          case KEY_STATE_OFF_CYCLE:
          default:
            if (millis() > autofire_timer[port_id][key_id]) {
              flip_state(port_id, key_id, RAPID_FIRE_PERIOD_ON, RAPID_FIRE_PERIOD_OFF);
            }
            break;
        }
      } else {
        #ifdef ENABLE_AUTO_FIRE
          /* Toggle autofire when mode is held and we've detected a released fire
          * button (ie. it is on in the last gamestate and not on anymore).
          */
          if (is_key_active(gamepad_state, SC_BTN_MODE)) {
            if (!is_key_active(gamepad_state, key_mask) && is_key_active(gamepad_last, key_mask)) {
              autofire_enabled[port_id][key_id] = !autofire_enabled[port_id][key_id];
            }
          }

          /* Handle auto fire */
          if (autofire_enabled[port_id][key_id]) {
            if (millis() > autofire_timer[port_id][key_id]) {
              flip_state(port_id, key_id, AUTO_FIRE_PERIOD_ON, AUTO_FIRE_PERIOD_OFF);
            }
          } else {
            /* Clear key state when button not used, this only run when it is not
            * under control by the alternate fire modes (auto fire or rapid fire).
            * Ensures that we don't end up with a button that is stuck in the held
            * position.
            */
            clear_state(port_id, key_id);
          }
        #else
          clear_state(port_id, key_id);
        #endif
      }
    }
  }

  /* Update and send the joystick state to the computer. */
  void update_joystick_turbo(const int port_id, const word gamepad_state, const word gamepad_last) {
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
      handle_turbo(port_id, KEY_C, SC_BTN_C, KEY_Z, SC_BTN_Z, gamepad_state, gamepad_last);
    }

    /* Swap A/B buttons around */
    if (swap_abxy) {
      handle_turbo(port_id, KEY_A, SC_BTN_B, KEY_X, SC_BTN_Y, gamepad_state, gamepad_last);
      handle_turbo(port_id, KEY_B, SC_BTN_A, KEY_Y, SC_BTN_X, gamepad_state, gamepad_last);    
    } else {
      handle_turbo(port_id, KEY_A, SC_BTN_A, KEY_X, SC_BTN_X, gamepad_state, gamepad_last);
      handle_turbo(port_id, KEY_B, SC_BTN_B, KEY_Y, SC_BTN_Y, gamepad_state, gamepad_last);
    }

    Joystick[port_id].setButton(KEY_START, is_key_active(gamepad_state, SC_BTN_START));
    #ifndef ENABLE_AUTO_FIRE
      Joystick[port_id].setButton(KEY_MODE, is_key_active(gamepad_state, SC_BTN_MODE));
    #endif
    Joystick[port_id].sendState();
  }

  /* Takes care of updating key states, update_joystick takes care of actually
  * sending the button states over USB.
  */
  void update_port_turbo(const int port_id) {
    switch (port_id) {
      case PORT_1:
        gamepad_1_state = gamepad_1.getState();
        update_joystick_turbo((swap_ports ? PORT_2 : PORT_1), gamepad_1_state, gamepad_1_last);
        gamepad_1_last = gamepad_1_state;
        break;

      case PORT_2:
      default:
        gamepad_2_state = gamepad_2.getState();
        update_joystick_turbo((swap_ports ? PORT_1 : PORT_2), gamepad_2_state, gamepad_2_last);
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
      update_port_turbo(port_id);
    }
  }
}