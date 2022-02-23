#include <Arduino.h>
#include <Joystick.h>
#include "constants.h"
#include "settings.h"
#include "led_control.h"
#include "key_mode.h"

extern unsigned long pwr_timer;
extern Joystick_ Joystick;
byte gamepad_state;
byte gamepad_last;

namespace mode_default {
  void init_mode() {
    pwr_timer = millis() + LED_SHUTOFF;
    pinMode(PIN_MODE, INPUT_PULLUP);

    /* Initialize pins used with controller */
    digitalWrite(PIN_CLK, LOW);
    pinMode(PIN_CLK, OUTPUT);
    digitalWrite(PIN_LATCH, LOW);
    pinMode(PIN_LATCH, OUTPUT);
    pinMode(PIN_DATA, INPUT);

    /* Initialize port */
    Joystick.begin(false);
    Joystick.setXAxisRange(-AXIS_RANGE, AXIS_RANGE);
    Joystick.setYAxisRange(-AXIS_RANGE, AXIS_RANGE);
  }
  
  bool is_key_active(const byte gamepad_state, const byte key) {
    return !bitRead(gamepad_state, key);
  }

  /* Update and send the joystick state to the computer. */
  void update_joystick(const byte gamepad_state, const byte gamepad_last) {
    if (gamepad_state != gamepad_last) {
      #ifdef PWR_ACTIVITY
        boost_pwr();
        pwr_timer = millis() + LED_FADE_DELAY;
      #endif
    }

    if (is_key_active(gamepad_state, KEY_LEFT) && is_key_active(gamepad_state, KEY_RIGHT)) Joystick.setXAxis(AXIS_NEUTRAL);
    else if (is_key_active(gamepad_state, KEY_LEFT)) Joystick.setXAxis(-AXIS_RANGE);
    else if (is_key_active(gamepad_state, KEY_RIGHT)) Joystick.setXAxis(AXIS_RANGE);
    else Joystick.setXAxis(AXIS_NEUTRAL);

    if (is_key_active(gamepad_state, KEY_UP) && is_key_active(gamepad_state, KEY_DOWN)) Joystick.setYAxis(AXIS_NEUTRAL);
    else if (is_key_active(gamepad_state, KEY_UP)) Joystick.setYAxis(-AXIS_RANGE);
    else if (is_key_active(gamepad_state, KEY_DOWN)) Joystick.setYAxis(AXIS_RANGE);
    else Joystick.setYAxis(AXIS_NEUTRAL);

    Joystick.setButton(SC_KEY_B, is_key_active(gamepad_state, KEY_B));
    Joystick.setButton(SC_KEY_A, is_key_active(gamepad_state, KEY_A));
    Joystick.setButton(SC_KEY_SELECT, is_key_active(gamepad_state, KEY_SELECT));
    Joystick.setButton(SC_KEY_START, is_key_active(gamepad_state, KEY_START));
    Joystick.setButton(SC_KEY_MODE, digitalRead(PIN_MODE) == LOW);

    Joystick.sendState();
  }

  /* Takes care of updating key states, before having update_joystick take care
   * of care of actually sending the button states over the USB-connection.
   */
  void update_port() {
    gamepad_state = 0xff;

    /* Have the controller latch data */
    digitalWrite(PIN_LATCH, HIGH);
    delayMicroseconds(PIN_SHIFT_DELAY);
    digitalWrite(PIN_LATCH, LOW);

    for (int i = 0; i < 8; i++) {
      bitWrite(gamepad_state, i, digitalRead(PIN_DATA));

      /* Pulse chip to load next bit */
      digitalWrite(PIN_CLK, HIGH);
      delayMicroseconds(PIN_SHIFT_DELAY);
      digitalWrite(PIN_CLK, LOW);
    }

    /* Update joystick state */
    update_joystick(gamepad_state, gamepad_last);
    gamepad_last = gamepad_state;
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
    
    update_port();
  }
}