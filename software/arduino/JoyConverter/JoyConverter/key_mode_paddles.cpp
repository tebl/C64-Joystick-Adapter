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

unsigned long paddle_timer[PORT_COUNT][2];
int paddle_value[PORT_COUNT][2];

void init_mode_paddles() {
  flash_pwr(3);

  pinMode(PIN_POT_AX, INPUT);
  pinMode(PIN_POT_AY, INPUT);
  pinMode(PIN_P1_LEFT, INPUT_PULLUP);
  pinMode(PIN_P1_RIGHT, INPUT_PULLUP);

  pinMode(PIN_POT_BX, INPUT);
  pinMode(PIN_POT_BY, INPUT);
  pinMode(PIN_P2_LEFT, INPUT_PULLUP);
  pinMode(PIN_P2_RIGHT, INPUT_PULLUP);

  /* Initialize port */
  for (int index = 0; index < PORT_COUNT; index++) {
    Joystick[index].setXAxisRange(200, 1000);
    Joystick[index].setYAxisRange(200, 1000);
    Joystick[index].begin(false);
  }
}


/* Update and send the joystick state to the computer. Given that some
 * controllers ordinarily does not allow two opposing directions to be pressed
 * at once, we'll explicitly filter those states (read as neutral instead of
 * favoring a direction).
 */
void send_paddle_status(const int port_id) {
  Joystick[port_id].setButton(0, is_waiting_release(port_id, KEY_LEFT));
  Joystick[port_id].setXAxis(paddle_value[port_id][AXIS_X]);

  Joystick[port_id].setButton(1, is_waiting_release(port_id, KEY_RIGHT));
  Joystick[port_id].setYAxis(paddle_value[port_id][AXIS_Y]);

  Joystick[port_id].sendState();
}

/* Get the analog input pin associated with the specified port and axis. */
int get_axis_pin(const int port_id, const int axis) {
  if (port_id == (swap_ports ? PORT_2 : PORT_1)) {
    if (axis == AXIS_X) return PIN_POT_AX;
    return PIN_POT_AY;
  } else {
    if (axis == AXIS_X) return PIN_POT_BX;
    return PIN_POT_BY;
  }
}

int get_distance(int value, int old_value) {
  if (value < old_value) return old_value - value;
  else return value - old_value;
}

/* Update the axis associated with the specified port. Note that we're using
 * the previous value if we are checking before the configured sample delay
 * has passed.
 */
void update_axis(const int port_id, const int axis) {
  if (millis() > paddle_timer[port_id][axis]) {
    int pin_number = get_axis_pin(port_id, axis);

    /* Read and discard, allow ADC time to settle. */
    analogRead(pin_number);
    delayMicroseconds(PADDLE_SAMPLE_DELAY);

    /* Read actual value, discard if not changed enough (jitter reduction) */
    int value = analogRead(pin_number);
    if (get_distance(value, paddle_value[port_id][axis]) > PADDLE_MIN_CHANGE) {
      paddle_value[port_id][axis] = value;
    }
    paddle_timer[port_id][axis] = millis() + PADDLE_SAMPLE_RATE;
  }
}

/* Takes care of updating key states, effectively debouncing the keys to avoid
 * jitter and then make sure updates are sent to the computer.
 */
void update_paddle(const int port_id) {
  update_axis(port_id, AXIS_X);
  debounce_joystick_key(port_id, KEY_LEFT);

  update_axis(port_id, AXIS_Y);
  debounce_joystick_key(port_id, KEY_RIGHT);

  send_paddle_status(port_id);
}

void handle_mode_paddles() {
  if (millis() > pwr_timer) {
    fade_pwr();
    pwr_timer = millis() + LED_FADE_SPEED;
  }

  for (int port_id = PORT_1; port_id < PORT_COUNT; port_id++) {
    update_paddle(port_id);
  }  
}