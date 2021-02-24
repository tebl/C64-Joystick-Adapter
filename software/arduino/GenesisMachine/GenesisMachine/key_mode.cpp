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