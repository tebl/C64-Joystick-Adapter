#ifndef _CONSTANTS_H
#define _CONSTANTS_H

/* 
 * Pin definitions 
 */
#define PIN_P1_UP 2
#define PIN_P1_DOWN 3
#define PIN_P1_LEFT 4
#define PIN_P1_RIGHT 5
#define PIN_P1_FIRE 15
#define PIN_POT_AY 19
#define PIN_POT_AX 18

#define PIN_P2_UP 6
#define PIN_P2_DOWN 7
#define PIN_P2_LEFT 8
#define PIN_P2_RIGHT 9
#define PIN_P2_FIRE 14
#define PIN_POT_BY 21
#define PIN_POT_BX 20

#define PIN_PWR 10
#define PIN_MODE 16

/* 
 * Definitions
 */
#define LED_OFF 0

#define KEY_MODE_DEFAULT 0
#define KEY_MODE_TURBO 1

#define PORT_1 0
#define PORT_2 1
#define PORT_COUNT 2

#define KEY_A 0
#define KEY_B 1
#define KEY_C 2
#define KEY_X 3
#define KEY_Y 4
#define KEY_Z 5
#define KEY_START 6
#define KEY_MODE 7
#define KEY_COUNT 8

/* Mainly just to keep code compatible between things. I know I'm lazy, but
 * free is free and that was the price you were given for all this!
 */
const int KEY_PINS[PORT_COUNT][KEY_COUNT] = { 
    { 0, 0, 0, 0, 0, 0, 0, PIN_MODE }, 
    { 0, 0, 0, 0, 0, 0, 0, PIN_MODE }
};

#define KEY_STATE_NEUTRAL 0
#define KEY_STATE_WAIT_RELEASE 1
#define KEY_STATE_ON_CYCLE 2
#define KEY_STATE_OFF_CYCLE 3
#endif