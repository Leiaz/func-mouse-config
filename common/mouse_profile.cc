#include "mouse_profile.h"

/**
 * The default constructor has reasonable default values (set in class
 * definition).
 */
MouseProfile::MouseProfile()
{
  m_buttons[WheelForward].setMouseAction(ACTION_SCROLL_UP);
  m_buttons[WheelBackward].setMouseAction(ACTION_SCROLL_DOWN);
  m_buttons[LeftButton].setMouseAction(ACTION_BUTTON_1);
  m_buttons[RightButton].setMouseAction(ACTION_BUTTON_2);
  m_buttons[WheelButton].setMouseAction(ACTION_BUTTON_3);
  m_buttons[SideButtonBack].setMouseAction(ACTION_BACKWARD);
  m_buttons[SideButtonFront].setMouseAction(ACTION_FORWARD);
  m_buttons[InstantAimButton].setMouseAction(ACTION_INSTANT_AIM);
  m_buttons[ProfileButton].setMouseAction(ACTION_PROFILE_CYCLE);
  m_buttons[DPIButtonFront].setMouseAction(ACTION_DPI_UP);
  m_buttons[DPIButtonBack].setMouseAction(ACTION_DPI_DOWN);

  m_buttons[ThumbButton].setMediaButton(MEDIA_PLAY_PAUSE);
 }

void MouseProfile::setName(const std::string &name)
{
  m_name = name;
}

std::string MouseProfile::getName() const
{
  return m_name;
}

void MouseProfile::setDPI(DPI s, unsigned char value)
{
  unsigned char v = value;

  if (v < MIN_DPI)
  {
    v = MIN_DPI;
  }
  else if (v>MAX_DPI)
  {
    v = MAX_DPI;
  }
  m_dpi_values[s] = v;
}

unsigned char MouseProfile::getDPI(DPI s) const
{
  return m_dpi_values[s];
}

void MouseProfile::setLiftOffDistance(unsigned char value)
{
  m_lift_off_distance = value;
}

unsigned char MouseProfile::getLiftOffDistance() const
{
  return m_lift_off_distance;
}

void MouseProfile::setAngleSnapping(bool enabled)
{
  m_angle_snapping = enabled;
}

bool MouseProfile::getAngleSnapping() const
{
  return m_angle_snapping;
}

void MouseProfile::setLEDState(LED led, LEDState value)
{
  m_led_state[led] = value;
}

MouseProfile::LEDState MouseProfile::getLEDState(LED led) const
{
  return m_led_state[led];
}

/* rgb_color = 0xRRGGBB */
void MouseProfile::setLEDColor(LED led, unsigned int rgb_color)
{
  m_led_color[led] = rgb_color;
}

unsigned int MouseProfile::getLEDColor(LED led) const
{
  return m_led_color[led];
}

ButtonSetting MouseProfile::getButtonSetting(MouseButton button) const
{
  return m_buttons[button];
}

void MouseProfile::setButton(MouseButton button, ButtonSetting setting)
{
  m_buttons[button] = setting;
}
