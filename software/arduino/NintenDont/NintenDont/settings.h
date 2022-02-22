#include "constants.h"

/*
 * Settings
 */
#define APP_TITLE "NintenDon't"
#define APP_VERSION "0.1"

#define LED_PWR_MAX 64               // Maximum brightness
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

/* Specify ALTERNATE_MODE to customize which mode is used when the mode button
 * on the device is held during powerup. At the moment there is probably no
 * alternative so leave it at this setting - or you could swap the values
 * around if you prefer. See FORCE_PRIMARY / FORCE_ALTERNATE to skip the
 * selection process on bootup.
 */
#define PRIMARY_MODE KEY_MODE_DEFAULT
// #define ALTERNATE_MODE KEY_MODE_DEFAULT

/* Uncomment one of these lines to force a specific mode, meaning that the
 * the startup timer allowing you to hold in the mode button is removed.
 */
#define FORCE_PRIMARY
//#define FORCE_ALTERNATE