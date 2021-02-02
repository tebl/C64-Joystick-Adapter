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
#define KEY_MODE_PADDLES 1

#define PORT_1 0
#define PORT_2 1
#define PORT_COUNT 2

#define KEY_UP 0
#define KEY_DOWN 1
#define KEY_LEFT 2
#define KEY_RIGHT 3
#define KEY_FIRE 4
#define KEY_ALT_FIRE 5
#define KEY_MODE 6

#define KEY_COUNT 7

#define AXIS_X 0
#define AXIS_Y 1

const int KEY_PINS[PORT_COUNT][KEY_COUNT] = { 
    { PIN_P1_UP, PIN_P1_DOWN, PIN_P1_LEFT, PIN_P1_RIGHT, PIN_P1_FIRE, PIN_POT_AX, PIN_MODE }, 
    { PIN_P2_UP, PIN_P2_DOWN, PIN_P2_LEFT, PIN_P2_RIGHT, PIN_P2_FIRE, PIN_POT_BX, PIN_MODE }
};

#define KEY_STATE_NEUTRAL 0
#define KEY_STATE_WAIT_RELEASE 1
#endif