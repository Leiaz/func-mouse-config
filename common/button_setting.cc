#include "button_setting.h"

/*
 *
 * Created using the hid_keyboard table from Linux/drivers/hid/hid-input.c
 *
  *  Copyright (c) 2000-2001 Vojtech Pavlik
  *  Copyright (c) 2006-2010 Jiri Kosina
  *
  *  HID to Linux Input mapping
*/
static const unsigned char hid_to_keycode[256] = {
//      0   1   2   3   4   5   6   7   8   9   A   B   C   D   E   F
        0,  0,  0,  0, 30, 48, 46, 32, 18, 33, 34, 35, 23, 36, 37, 38,// 0
       50, 49, 24, 25, 16, 19, 31, 20, 22, 47, 17, 45, 21, 44,  2,  3,// 1
        4,  5,  6,  7,  8,  9, 10, 11, 28,  1, 14, 15, 57, 12, 13, 26,// 2
       27, 43, 43, 39, 40, 41, 51, 52, 53, 58, 59, 60, 61, 62, 63, 64,// 3
       65, 66, 67, 68, 87, 88, 99, 70,119,110,102,104,111,107,109,106,// 4
      105,108,103, 69, 98, 55, 74, 78, 96, 79, 80, 81, 75, 76, 77, 71,// 5
       72, 73, 82, 83, 86,127,116,117,183,184,185,186,187,188,189,190,// 6
      191,192,193,194,134,138,130,132,128,129,131,137,133,135,136,113,// 7
      115,114,  0,  0,  0,121,  0, 89, 93,124, 92, 94, 95,  0,  0,  0,// 8
      122,123, 90, 91, 85,  0,  0,  0,  0,  0,  0,  0,111,  0,  0,  0,// 9
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,// A
        0,  0,  0,  0,  0,  0,179,180,  0,  0,  0,  0,  0,  0,  0,  0,// B
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,// C
        0,  0,  0,  0,  0,  0,  0,  0,111,  0,  0,  0,  0,  0,  0,  0,// D
       29, 42, 56,125, 97, 54,100,126,164,166,165,163,161,115,114,113,// E
      150,158,159,128,136,177,178,176,142,152,173,140,  0,  0,  0,  0 // F
 };

unsigned char ButtonSetting::getHIDFromKeycode(unsigned char keycode)
{
  for (unsigned char hid=0;hid<256;hid++)
  {
    if (hid_to_keycode[hid]==keycode)
    {
      return hid;
    }
  }
  return 0;
}

/* Kernel keycode = xkeycode - 8
 */
unsigned char ButtonSetting::getHIDFromXKeycode(unsigned char xkeycode)
{
  if (xkeycode<8) return 0;

  return getHIDFromKeycode(xkeycode - 8);
}

ButtonSetting::ButtonSetting(Type type)
:m_type(type)
{
}

ButtonSetting::Type ButtonSetting::getType() const
{
  return m_type;
}

unsigned char ButtonSetting::getValue() const
{
  if (m_type == Macro)
  {
    return 0;
  }
  return m_value;
}

void ButtonSetting::setMouseAction(unsigned char action)
{
  m_type = MouseAction;
  m_value = action;
}

void ButtonSetting::setMediaButton(unsigned char media)
{
  m_type = MediaButton;
  m_value = media;
}

void ButtonSetting::setShortcut(unsigned char shortcut)
{
  m_type = CommonShortcut;
  m_value = shortcut;
}

void ButtonSetting::setSingleKey(unsigned char hid_key)
{
  m_type = SingleKey;
  m_value = hid_key;
}

void ButtonSetting::setDisabled()
{
  m_type = Disabled;
  m_macro.clear();
  m_value = 0;
}

void ButtonSetting::setMacro(const std::vector<unsigned char> &macro)
{
  m_type = Macro;
  m_macro = macro;
}

std::vector<unsigned char> ButtonSetting::getMacro() const
{
  return m_macro;
}
