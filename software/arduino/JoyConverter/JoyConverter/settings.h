#include "constants.h"

/*
 * Settings
 */
#define APP_TITLE "JoyKEY JoyConverter"
#define APP_VERSION "0.2"

#define LED_PWR_MAX 128               // Maximum brightness
#define LED_SHUTOFF 5000              // Time after boot when fade starts
#define LED_FADE_SPEED 500            // Delay between LED fade steps
#define LED_STEP 8                    // Change per step when fading
#define LED_FLASH_DELAY 100           // Delay between blink steps
#define BOOT_MODE_THRESHOLD 1000      // Time needed before alternate mode
#define DEBOUNCE_DELAY 10             // Debounce delay for keys

/* Paddles are analog inputs and they need to be sampled twice, with a short
   delay while the Arduino input stabilizes (this is PADDLE_SAMPLE_DELAY).
   PADDLE_SAMPLE_RATE is a timer deciding how often we'll attempt to read the
   values.
*/
#define PADDLE_SAMPLE_DELAY 10        // First read is discarded, time to settle.
#define PADDLE_SAMPLE_RATE 50         // Ideal time to wait between updates
#define PADDLE_MIN_CHANGE 40          // Increase to decrease resolution,
                                      // reduces the jitter from the signal.

/* When uncommented this allows the PWR light to show joystick activity,
 * it'll grow brighter with movement and fade over time.
 */
#define PWR_ACTIVITY
#define PWR_ACTIVITY_MIN 8            // Defines a minimum activity LED level,
                                      // uncomment to allow fading out completely.

/* Specifies the minimum hold time of the magic key-combination to enable
 * alternate mode. BOOT_DELAY specifies the length of the pause given the
 * user to release the key combination.
 */
#define BOOT_MODE_THRESHOLD 1000
#define BOOT_DELAY 1000

/* Specify KEY_MODE_JOYSTICK to have the alternate mode activate the autofire
 * function.
 */
#define ALTERNATE_MODE KEY_MODE_PADDLES

/* Uncomment this line to force the alternate mode, meaning that the magic
 * key combination requirement on startup is removed completely.
 */
//#define FORCE_ALTERNATE