#include "constants.h"

/*
 * Settings
 */
#define APP_TITLE "GenesisMachine"
#define APP_VERSION "0.2"

#define LED_PWR_MAX 128               // Maximum brightness
#define LED_SHUTOFF 3000              // Time after boot when fade starts
#define LED_FADE_SPEED 250            // Delay between LED fade steps
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
#define ALTERNATE_MODE KEY_MODE_TURBO

/* Uncomment one of these lines to force a specific mode, meaning that the
 * the startup timer allowing you to hold in the mode button is removed.
 */
//#define FORCE_DEFAULT
//#define FORCE_ALTERNATE

/* These numbers tune the time autofire and rapid fire spend in each of the
 * phases, meaning they specify how much time the fire button is held in and
 * how much time until it is released again. 1000 divided by both periods added
 * together gives you the value in Hz.
 * 
 * Rapid fire is only used on the alternate turbo mode, it remaps the XYZ
 * buttons so that they function as rapid fire buttons for ABC. Auto fire is
 * available in both modes, it is enabled by holding mode and pushing the ABC
 * buttons (disables the mode button). Remove ENABLE_AUTO_FIRE below to disable
 * this, allowing the mode button to be used as normal.
 */
#define ENABLE_AUTO_FIRE
#define AUTO_FIRE_PERIOD_ON 80
#define AUTO_FIRE_PERIOD_OFF 80
#define RAPID_FIRE_PERIOD_ON 40
#define RAPID_FIRE_PERIOD_OFF 40