/*
 * joystick.h
 *
 * Joystick routines to handle arcade to usb conversion
 *  
 * Version 1.1 (See CHANGELOG)
 * Release date: 2019-04-19
 *  
 * Written & released by M.F. Wieland (TSB)
 *  
 *  Licensed under the MIT License. See LICENSE file in the project root for full license information.  
 */

#ifndef JOYSTICK_h
#define JOYSTICK_h

#include "HID.h"

//================================================================================
//  Joystick
//================================================================================

class Joystick_
{
private:
  uint16_t _buttons = 0x0;
  uint8_t _x_axis = 0x7F;
  uint8_t _y_axis = 0x7F;
  
public:
  
  Joystick_(void);

  void buttons(uint16_t b);
  void reset(void);
  void up();
  void right();
  void down();
  void left();
  void button_press(uint16_t b);
  void usb_update(); 
};
extern Joystick_ Joystick;

#endif
