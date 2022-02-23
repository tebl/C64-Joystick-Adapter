#ifndef _CONSTANTS_H
#define _CONSTANTS_H

/* 
 * Pin definitions, these identify the digital pin numbers used by the Arduino.
 * The number in the comment behind the line is the pin number on the 
 * IDC-connector. I would have wanted to put wire colours here, but the
 * extension cables are pretty random in that regards so you'll need a
 * multimeter to figure those out.
 */
#define PIN_CLK 6       /*       J3 IDC pin 1 */
#define PIN_LATCH 7     /*       J3 IDC pin 2 */
#define PIN_DATA 8      /*       J3 IDC pin 3 */
                        /* VCC - J3 IDC pin 7 */
                        /* GND - J3 IDC pin 8 */
#define PIN_SHIFT_DELAY 20

#define PIN_PWR 10
#define PIN_MODE 16

/* 
 * Definitions
 */
#define LED_OFF 0
#define KEY_MODE_DEFAULT 0
#define AXIS_RANGE 1
#define AXIS_NEUTRAL 0

/* These are the button IDs seen on computer */
#define SC_KEY_B 0
#define SC_KEY_A 1
#define SC_KEY_SELECT 2
#define SC_KEY_START 3
#define SC_KEY_MODE 4

/* These button IDs are used when referring to the byte data coming from the
 * NES controller.
 */
#define KEY_A 0
#define KEY_B 1
#define KEY_SELECT 2
#define KEY_START 3
#define KEY_UP 4
#define KEY_DOWN 5
#define KEY_LEFT 6
#define KEY_RIGHT 7
#define KEY_COUNT 8
#endif