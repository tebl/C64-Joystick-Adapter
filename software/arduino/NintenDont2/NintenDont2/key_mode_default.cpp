#include <Arduino.h>
#include "Gamepad.h"
#include "constants.h"
#include "settings.h"
#include "led_control.h"
#include "key_mode.h"

extern unsigned long pwr_timer;
byte gamepad_state;
byte gamepad_last;

const char *gp_serial = "Copybite Arcade";

Gamepad_ Gamepad;           // Set up USB HID gamepad
bool usb_update = true;     // Should gamepad data be sent to USB?

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

    Gamepad.reset();
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

      usb_update = true;
    }

    Gamepad._GamepadReport.Y = -1;
    if (is_key_active(gamepad_state, KEY_LEFT) && is_key_active(gamepad_state, KEY_RIGHT)) Gamepad._GamepadReport.X = AXIS_NEUTRAL;
    else if (is_key_active(gamepad_state, KEY_LEFT)) Gamepad._GamepadReport.X = -AXIS_RANGE;
    else if (is_key_active(gamepad_state, KEY_RIGHT)) Gamepad._GamepadReport.X = AXIS_RANGE;
    else Gamepad._GamepadReport.X = AXIS_NEUTRAL;

    if (is_key_active(gamepad_state, KEY_UP) && is_key_active(gamepad_state, KEY_DOWN)) Gamepad._GamepadReport.Y = AXIS_NEUTRAL;
    else if (is_key_active(gamepad_state, KEY_UP)) Gamepad._GamepadReport.Y = -AXIS_RANGE;
    else if (is_key_active(gamepad_state, KEY_DOWN)) Gamepad._GamepadReport.Y = AXIS_RANGE;
    else Gamepad._GamepadReport.Y = AXIS_NEUTRAL;

    Gamepad._GamepadReport.buttons = (~gamepad_state & 0x0f);
    if (digitalRead(PIN_MODE) == LOW) Gamepad._GamepadReport.buttons = Gamepad._GamepadReport.buttons | 0x10;

    if (usb_update) {
      Gamepad.send();
      usb_update = false;
    }
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