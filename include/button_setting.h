#ifndef BUTTON_SETTING_H
#define BUTTON_SETTING_H

#include <vector>

/**
 * Default actions that can be assigned to mouse buttons
 * (Change to enum ?)
 */
#define ACTION_BUTTON_1 0xf0 /// evdev button 1 / "Left" click
#define ACTION_BUTTON_2 0xf1 /// evdev button 2 / "Right" click
#define ACTION_BUTTON_3 0xf2 /// evdev button 3 / "Middle" click
#define ACTION_FORWARD 0xf3
#define ACTION_BACKWARD 0xf4
#define ACTION_SCROLL_UP 0xfa
#define ACTION_SCROLL_DOWN 0xfb
#define ACTION_DPI_UP 0xe9
#define ACTION_DPI_DOWN 0xea
#define ACTION_DPI_CYCLE 0xe8
#define ACTION_INSTANT_AIM 0xeb
#define ACTION_PROFILE_CYCLE 0xec
#define ACTION_PROFILE_UP 0xdd
#define ACTION_PROFILE_DOWN 0xee

/*
Media :
 - Play/Pause      04 00 00 CD
 - Volume Up       04 00 00 E9
 - Volume Down     04 00 00 EA
 - Mute            04 00 00 E2
 - Next Track      04 00 00 B5
 - Previous track  04 00 00 B6

 Numbers correspond to HID Usage
 */
#define MEDIA_PLAY_PAUSE 0xcd
#define MEDIA_VOLUME_UP 0xe9
#define MEDIA_VOLUME_DOWN 0xea
#define MEDIA_MUTE 0xe2
#define MEDIA_NEXT_TRACK 0xb5
#define MEDIA_PREVIOUS_TRACK 0xb6

/*
"Windows" commands :
 - Open             Ctrl + O   55 00 00 01
 - Copy             Ctrl + C   55 00 00 02
 - Paste            Ctrl + V   55 00 00 03
 - Cut              Ctrl + X   55 00 00 04
 - Undo             Ctrl + Z   55 00 00 05
 - Redo      Ctrl + Shift + Z  55 00 00 06
 - Save             Ctrl + S   55 00 00 07
 - Select all       Ctrl + A   55 00 00 08
 - Find             Ctrl + F   55 00 00 09
 - New              Ctrl + N   55 00 00 0A

 Arbitrary numbers ? Change to enum ?
*/
#define SHORTCUT_CTRL_O 0x01
#define SHORTCUT_CTRL_C 0x02
#define SHORTCUT_CTRL_V 0x03
#define SHORTCUT_CTRL_X 0x04
#define SHORTCUT_CTRL_Z 0x05
#define SHORTCUT_CTRL_SHIFT_Z 0x06
#define SHORTCUT_CTRL_S 0x07
#define SHORTCUT_CTRL_A 0x08
#define SHORTCUT_CTRL_F 0x09
#define SHORTCUT_CTRL_N 0x0a

class ButtonSetting
{
  public:
    enum Type
    {
      MouseAction,
      MediaButton,
      CommonShortcut,
      SingleKey,
      Macro,
      Disabled
    };

    ButtonSetting(Type type = Disabled);

    static unsigned char getHIDFromKeycode(unsigned char keycode);
    static unsigned char getHIDFromXKeycode(unsigned char xkeycode);

    Type getType() const;
    unsigned char getValue() const;

    void setMouseAction(unsigned char action);
    void setMediaButton(unsigned char media);
    void setShortcut(unsigned char shortcut);
    void setSingleKey(unsigned char hid_key);
    void setDisabled();
    void setMacro(const std::vector<unsigned char> &macro);

    std::vector<unsigned char> getMacro() const;

  private:
    Type m_type;
    unsigned char m_value;
    std::vector<unsigned char> m_macro;
};
#endif
