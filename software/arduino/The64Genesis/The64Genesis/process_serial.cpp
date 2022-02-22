#include <Arduino.h>
#include "constants.h"
#include "settings.h"

void init_serial() {
  Serial.begin(115200);

  /* Disable TX and RX LEDs */  
  pinMode(LED_BUILTIN_TX, INPUT);
  pinMode(LED_BUILTIN_RX, INPUT);

  Serial.print(APP_TITLE);
  Serial.print(" ");
  Serial.println(APP_VERSION);
}