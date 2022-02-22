#include <Arduino.h>
#include "constants.h"
#include "settings.h"
#include "led_control.h"
#include "key_mode.h"

bool boot_done = false;
unsigned long boot_timeout = 0;
unsigned long boot_detect = 0;

 void init_keys() {
  boot_timeout = millis() + 2000;

  /* 
   * Probably won't be stable without pullups, but we'll have to start
   * somewhere instead of something that might kill a connected system.
   */
  pinMode(PIN_MODE, INPUT_PULLUP);
 }

bool check_boot() {
  if (millis() > boot_timeout) {
    return init_mode(PRIMARY_MODE);
  } else {
    /* Check if mode is continuously held down  */
    if (digitalRead(PIN_MODE) == LOW) {
      if (boot_detect == 0) boot_detect = millis();
    } else boot_detect = 0;
  }
  return false;
}

void process_keys() {
  #ifdef FORCE_ALTERNATE
    if (!boot_done) init_mode(ALTERNATE_MODE);
    else handle_mode();
  #else
    #ifdef FORCE_PRIMARY
      if (!boot_done) init_mode(PRIMARY_MODE);
      else handle_mode();
    #else
      if (!boot_done) check_boot();
      else {
        handle_mode();
      }
    #endif
  #endif
}