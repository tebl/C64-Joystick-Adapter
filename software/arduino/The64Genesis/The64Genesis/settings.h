#include "constants.h"

/*
 * Settings
 */
#define APP_TITLE "The64Genesis"
#define APP_VERSION "0.1"

#define LED_PWR_MAX 128               // Maximum brightness
#define LED_SHUTOFF 3000              // Time after boot when fade starts
#define LED_FADE_SPEED 250            // Delay between LED fade steps
#define LED_FADE_DELAY 1000           // Time before fading starts
#define LED_STEP 4                    // Change per step when fading
#define LED_FLASH_DELAY 100           // Delay between blink steps
#define BOOT_MODE_THRESHOLD 1000      // Time needed before alternate mode
#define DEBOUNCE_DELAY 10             // Debounce delay for keys

/* These settings changes the default behavior, mainly to make the device
 * feel more familiar to people using a Mega Drive adapter with any of the 
 * Commodore computers (either C64 Joystick Switcher or
 * OpenC64OpenMegaDrivePadAdapter).
 * 
 * SWAP_PORTS makes port 2 the main controller port similar to what is, 
 * almost, standard on the Commodore 64. C_TO_JUMP disables up direction
 * on the gamepad, reassigns C button to jump instead (makes things easier
 * in racing games and platformers) - this can also be toggled by pushing the
 * MODE button on the device itself. Use SWAP_ABXY to change button order,
 * this - again - to match the mentioned adapters. See ENABLE_AUTO_FIRE at the
 * bottom if looking for a way to use the mode button on the controller as
 * normal.
 */
#define SWAP_PORTS                    // Swap around ports 1/2
//#define C_TO_JUMP                   // C is now the UP button, MODE to toggle.
//#define SWAP_AB                     // Swap A/B

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
 * alternative so leave it at this setting.
 */
#define PRIMARY_MODE KEY_MODE_DEFAULT
#define ALTERNATE_MODE KEY_MODE_TURBO

/* Uncomment one of these lines to force a specific mode, meaning that the
 * the startup timer allowing you to hold in the mode button is removed.
 */
#define FORCE_PRIMARY
//#define FORCE_ALTERNATE

/* These numbers tune the time autofire and rapid fire spend in each of the
 * phases, meaning they specify how much time the fire button is held in and
 * how much time until it is released again. 1000 divided by both periods added
 * together gives you an approximate value in Hz.
 * 
 * Rapid fire is only used on the alternate turbo mode, it remaps the XYZ
 * buttons so that they function as rapid fire buttons for ABC. Auto fire is
 * available in both modes, it is enabled by holding mode and pushing the ABC
 * buttons (disables the mode button). Remove ENABLE_AUTO_FIRE below to disable
 * this, allowing the mode button to be used as a "normal" button.
 */
#define ENABLE_AUTO_FIRE
#define AUTO_FIRE_PERIOD_ON 80
#define AUTO_FIRE_PERIOD_OFF 80
#define RAPID_FIRE_PERIOD_ON 40
#define RAPID_FIRE_PERIOD_OFF 40