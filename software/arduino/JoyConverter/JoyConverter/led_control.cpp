#include <Arduino.h>
#include "constants.h"
#include "settings.h"

#define LED_INDEX_PWR 0

int current_pwr = LED_PWR_MAX;

int led_values[] = { LED_PWR_MAX };
const int led_max[] = { LED_PWR_MAX };
const int led_pins[] = { PIN_PWR }; 

/* Updates the LED as indicated by the index. Values will be forced within
 * 0 and max_value, making things a bit easier in other code - note that a
 * maximum value of 0 will always take precedence (can never be turned on). 
 */
void set_led(const int index, int value) {
  if (value < LED_OFF) value = LED_OFF;
  if (value > led_max[index]) value = led_max[index];

  led_values[index] = value;
  analogWrite(led_pins[index], value);
}

/* Set LED state, this just toggles between full value and completely off. */
void set_led(const int index, const bool value) {
  set_led(index, (value ? led_max[index] : LED_OFF));
}

/* Fades out the LED indicated using index. */
void fade_led(const int index, const int step_size) {
  if (led_values[index] == LED_OFF) return;
  set_led(index, led_values[index] - step_size);
}

/* Same as above, except we ensure we stay above the specified lower limit. All
 * of this is ignored if the LED has been permanently set to be in the off state.
 */
void fade_led(const int index, const int minimum, const int step_size) {
  if (led_values[index] == LED_OFF && led_max[index] == LED_OFF) return;
  int value = led_values[index] - step_size;
  if (value < minimum) value = minimum;
  set_led(index, value);
}

/* The opposite of fade_led. */
void boost_led(const int index, const int step_size) {
  set_led(index, led_values[index] + step_size);
}

/* Check if a specific LED has been turned on */
bool is_led_on(const int index) {
  return led_values[index] > LED_OFF;
}

/* Delay used when spacing out user feedback. Note that this is a blocking
 * function, so it'll probably not be much use when we want to respond to
 * keypresses in due time. */
void flash_delay() { delay(LED_FLASH_DELAY); }

void flash_led(const int index, const int num_flashes) {
  for (int i = 0; i < num_flashes; i++) {
  if (i > 0) flash_delay();
      set_led(index, true);
      flash_delay();
      set_led(index, false);
  }
}

bool is_pwr_on() { return is_led_on(LED_INDEX_PWR); }
void set_pwr(int value) { set_led(LED_INDEX_PWR, value); }
void set_pwr(bool value) { set_led(LED_INDEX_PWR, value); }
void fade_pwr() { fade_led(LED_INDEX_PWR, LED_STEP); }
void fade_pwr(const int minimum) { fade_led(LED_INDEX_PWR, minimum, LED_STEP); }
void boost_pwr() { boost_led(LED_INDEX_PWR, LED_STEP); }
void flash_pwr(int num_flashes) { flash_led(LED_INDEX_PWR, num_flashes); }

void init_led() {
  pinMode(PIN_PWR, OUTPUT);
  set_pwr(true);
}