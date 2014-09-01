#ifndef MOUSE_PROFILE_H
#define MOUSE_PROFILE_H

#include "button_setting.h"

#include <memory>
#include <string>

#define MIN_DPI 0x01 /// Minimum DPI setting value
#define MAX_DPI 0x3f /// Maximum DPI setting value
#define DPI_STEP 90 ///

#define LIFT_OFF_DISTANCE_LOW 0x00
#define LIFT_OFF_DISTANCE_MEDIUM 0x01
#define LIFT_OFF_DISTANCE_HIGH 0x02

#define BUTTON_COUNT 12
#define DPI_COUNT 8
#define LED_COUNT 2

class MouseProfile
{
  public:
    /**
     * Index of the button settings in the buttons array.
     * In the order they are sent to the mouse.
     */
    enum MouseButton
    {
      WheelForward,
      WheelBackward,
      LeftButton ,
      RightButton,
      WheelButton,
      SideButtonBack,
      SideButtonFront,
      InstantAimButton,
      ThumbButton,
      ProfileButton,
      DPIButtonFront,
      DPIButtonBack,
    };

    /**
     */
    enum DPI
    {
      DPIOneX,
      DPIOneY,
      DPITwoX,
      DPITwoY,
      DPIThreeX,
      DPIThreeY,
      DPIInstantAimX,
      DPIInstantAimY,
    };

    enum LED
    {
      SideLED,
      WheelLED
    };

    enum LEDState
    {
      LEDOn,
      LEDOff,
      LEDPulsate
    };

    MouseProfile();
    ~MouseProfile() = default;
    MouseProfile(const MouseProfile&) = default;
    MouseProfile& operator=(const MouseProfile&) = default;
    MouseProfile(MouseProfile&&) = default;
    MouseProfile& operator=(MouseProfile&&) = default;

    void setName(const std::string &name);
    std::string getName() const;
    /**
     *  DPI values range from 0x01 to 0x3f
     */
    void setDPI(DPI s, unsigned char value);

    unsigned char getDPI(DPI s) const;

    /** 0x00=low 0x01=med 0x02=high */
    void setLiftOffDistance(unsigned char value);

    unsigned char getLiftOffDistance() const;

    /**
     * \param enabled 0x00=disabled 0x01=enabled
     */
    void setAngleSnapping(bool enabled);

    bool getAngleSnapping() const;

    /**
     * \param rgb_color = 0xRRGGBB
     */
    void setLEDColor(LED led, unsigned int rgb_color);

    unsigned int getLEDColor(LED led) const;

    void setLEDState(LED led, LEDState value);

    LEDState getLEDState(LED led) const;

    ButtonSetting getButtonSetting(MouseButton button) const;
    void setButton(MouseButton button, ButtonSetting setting);

  private:
    /** Default values make sense as a working profile.
     */
    std::string m_name = "noname";
    unsigned char m_dpi_values[DPI_COUNT] = {0x0b,0x0b,0x20,0x20,0x3f,0x3f};
    unsigned char m_lift_off_distance = 0;
    bool m_angle_snapping = false;
    LEDState m_led_state[LED_COUNT] = {LEDOn, LEDOn};
    unsigned int m_led_color[LED_COUNT] = {0x00ff00, 0x0000ff};
    ButtonSetting m_buttons[BUTTON_COUNT];
};
#endif
