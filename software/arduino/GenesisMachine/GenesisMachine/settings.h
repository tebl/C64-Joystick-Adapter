#include "constants.h"

/*
 * Settings
 */
#define APP_TITLE "GenesisMachine"
#define APP_VERSION "0.2"

#define LED_PWR_MAX 128               // Maximum brightness
#define LED_SHUTOFF 5000              // Time after boot when fade starts
#define LED_FADE_SPEED 500            // Delay between LED fade steps
#define LED_FADE_DELAY 1000           // Time before fading starts
#define LED_STEP 4                    // Change per step when fading
#define LED_FLASH_DELAY 100           // Delay between blink steps
#define BOOT_MODE_THRESHOLD 1000      // Time needed before alternate mode
#define DEBOUNCE_DELAY 10             // Debounce delay for keys

/* When uncommented this allows the PWR light to show joystick activity,
 * it'll grow brighter with movement and fade over time.
 */
#define PWR_ACTIVITY
#define PWR_ACTIVITY_MIN 8            // Defines a minimum activity LED level,
                                      // uncomment to allow fading out completely.
#define LINK_AUTOFIRE_PWR             // PWR LED flickers in sync with autofire

/* Specifies the minimum hold time of the magic key-combination to enable
 * alternate mode. BOOT_DELAY specifies the length of the pause given the
 * user to release the key combination.
 */
#define BOOT_MODE_THRESHOLD 1000
#define BOOT_DELAY 1000

/* Specify KEY_MODE_JOYSTICK to have the alternate mode activate the autofire
 * function.
 */
#define ALTERNATE_MODE KEY_MODE_AUTOFIRE

/* Uncomment this line to force the alternate mode, meaning that the magic
 * key combination requirement on startup is removed completely.
 */
//#define FORCE_ALTERNATE

/* These numbers tune the time autofire and rapid fire spend in each of the
 * phases, meaning they specify how much time the fire button is held in and
 * how much time until it is released again. 1000 divided by both periods added
 * together gives you the value in Hz.
 */
#define AUTO_FIRE_PERIOD_ON 80
#define AUTO_FIRE_PERIOD_OFF 80
#define RAPID_FIRE_PERIOD_ON 60
#define RAPID_FIRE_PERIOD_OFF 60