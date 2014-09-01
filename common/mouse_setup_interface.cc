#include "mouse_setup_interface.h"

#include <string.h> //memcpy
#include <iostream>

#define FUNC_MOUSE_VENDOR_ID 0x040b // "Weltrend semiconductor"
#define FUNC_MOUSE_PRODUCT_ID 0x0a68
#define FIRMWARE_UPGRADE_PRODUCT_ID 0x6873

#define USB_TIMEOUT 1000 // ms

#define DPI_1_X_INDEX 0
#define DPI_1_Y_INDEX 1
#define DPI_2_X_INDEX 2
#define DPI_2_Y_INDEX 3
#define DPI_3_X_INDEX 4
#define DPI_3_Y_INDEX 5
#define DPI_INSTANT_AIM_X_INDEX 20
#define DPI_INSTANT_AIM_Y_INDEX 21
#define LIFT_OFF_DISTANCE_INDEX 6
#define ANGLE_SNAPPING_INDEX 7
#define MACRO_BITS_0_7_INDEX 8
#define MACRO_BITS_8_11_INDEX 9
#define WHEEL_LED_RED_INDEX 10
#define WHEEL_LED_GREEN_INDEX 11
#define WHEEL_LED_BLUE_INDEX 12
#define WHEEL_LED_STATE_INDEX 13
#define SIDE_LED_RED_INDEX 15
#define SIDE_LED_GREEN_INDEX 16
#define SIDE_LED_BLUE_INDEX 17
#define SIDE_LED_STATE_INDEX 18

#define LED_STATE_OFF 0x00
#define LED_STATE_ON 0x03
#define LED_STATE_PULSATE 0x04

bool MouseSetupInterface::isSupportedMouse(libusb_device *device)
{
  struct libusb_device_descriptor descriptor;
  int err;
  err = libusb_get_device_descriptor(device, &descriptor);
  if (err)
  {
    setLibUSBError("Error libusb_get_device_descriptor :",err);
    return false;
  }
  if ((descriptor.idVendor == FUNC_MOUSE_VENDOR_ID)&&
      (descriptor.idProduct == FUNC_MOUSE_PRODUCT_ID)&&
      ((descriptor.bcdDevice == 0x112)||(descriptor.bcdDevice==0x11c)))
    {
      return true;
    }
  return false;
}

MouseSetupInterface::MouseSetupInterface()
{
  libusb_init(&m_context);
  findMouse();
}

MouseSetupInterface::~MouseSetupInterface()
{
  releaseAndCloseHandle();
  libusb_exit(m_context);
}

bool MouseSetupInterface::findMouse()
{
  int count = 0;
  libusb_device **list = nullptr;
  libusb_device *mouse_device = nullptr;
  int err = 0;

  releaseAndCloseHandle();
  count = libusb_get_device_list(nullptr, &list);
  if (count<0)
  {
    setLibUSBError("Error getting device list : ", count);
    return false;
  }

  for (int i = 0; i< count; i++)
  {
    if (isSupportedMouse(list[i]))
    {
      mouse_device = list[i];
      break;
    }
  }

  if (mouse_device)
  {
    struct libusb_device_descriptor desc;
    libusb_get_device_descriptor(mouse_device, &desc);
    m_mouse_version = desc.bcdDevice;
    err = libusb_open(mouse_device, &m_handle);
    if (err)
    {
      setLibUSBError("Error libusb_open : ",err);
    }
  } else {
    setError("Mouse not found.");
  }

  libusb_free_device_list(list, 1);

  if (!m_handle) // If no mouse found or open failed
  {
    return false;
  }

  err = libusb_set_auto_detach_kernel_driver(m_handle, 1);
  if(err)
  {
    setLibUSBError("Error libusb_set_auto_detach_kernel_driver :",err);
    goto failed;
  }

  err = libusb_claim_interface(m_handle, 2);
  if(err)
  {
    setLibUSBError("Error libusb_claim_interface : ",err);
    goto failed;
  }
  return true;

failed:
  libusb_close(m_handle);
  m_handle = nullptr;
  return false;
}

/**
*/
bool MouseSetupInterface::setPollingInterval(unsigned char interval)
{
  unsigned char command_buffer[32] = {0};
  command_buffer[0]=0x07;
  command_buffer[1]=0x04;

  command_buffer[2]=interval;

  return setReport(command_buffer, 32);
}

/**
 *
 * \param profile_num Profile number : 0x01, 0x02 or 0x03
 */
bool MouseSetupInterface::switchToProfile(unsigned char profile_num)
{
  unsigned char command_buffer[32] = {0};
  command_buffer[0]=0x07;
  command_buffer[1]=0x01;
  if ((profile_num > 0x03)||(profile_num < 0x01))
  {
    return false;
  }
  command_buffer[2] = profile_num;

  return setReport(command_buffer, 32);
}

/* DPI num are 1, 2 or 3 */
bool MouseSetupInterface::setSelectedDPI(unsigned char dpi_profile1,
                     unsigned char dpi_profile2,
                     unsigned char dpi_profile3)
{
  if (((dpi_profile1 < 0x01) || (dpi_profile1 > 0x03)) ||
      ((dpi_profile2 < 0x01) || (dpi_profile2 > 0x03)) ||
      ((dpi_profile3 < 0x01) || (dpi_profile3 > 0x03)))
  {
    return false;
  }

  unsigned char command_buffer[32] = {0};
  command_buffer[0]=0x07;
  command_buffer[1]=0x03;
  command_buffer[2] = dpi_profile1;
  command_buffer[3] = dpi_profile2;
  command_buffer[4] = dpi_profile3;

  return setReport(command_buffer, 32);
}

bool MouseSetupInterface::setProfile(const MouseProfile &p,
                unsigned char profile_number)
{
  size_t button_size = 96;

  unsigned char command_buffer[32] = {0};
  unsigned char settings_buffer[32] = {0};
  unsigned char button_buffer[12][512] = {{0}};
  ButtonSetting b;
  unsigned int color;
  unsigned char led_state;

  /**
   * With firmware 1.12 mouse expects 96 bytes per button
   * With firmware 1.1c mouse expects 512 bytes
   */
  if (m_mouse_version == 0x11c)
  {
    button_size = 512;
  } else if (m_mouse_version == 0x112){
    button_size = 96;
  }

  /* Get message to send to mouse from profile */

  settings_buffer[14] = 0x05;// XXX Mean something ?
  settings_buffer[19] = 0x05;
  settings_buffer[DPI_1_X_INDEX] = p.getDPI(MouseProfile::DPIOneX);
  settings_buffer[DPI_1_Y_INDEX] = p.getDPI(MouseProfile::DPIOneY);
  settings_buffer[DPI_2_X_INDEX] = p.getDPI(MouseProfile::DPITwoX);
  settings_buffer[DPI_2_Y_INDEX] = p.getDPI(MouseProfile::DPITwoY);
  settings_buffer[DPI_3_X_INDEX] = p.getDPI(MouseProfile::DPIThreeX);
  settings_buffer[DPI_3_Y_INDEX] = p.getDPI(MouseProfile::DPIThreeY);
  settings_buffer[DPI_INSTANT_AIM_X_INDEX] =
    p.getDPI(MouseProfile::DPIInstantAimX);
  settings_buffer[DPI_INSTANT_AIM_Y_INDEX] =
    p.getDPI(MouseProfile::DPIInstantAimY);
  settings_buffer[LIFT_OFF_DISTANCE_INDEX] = p.getLiftOffDistance();
  settings_buffer[ANGLE_SNAPPING_INDEX] = p.getAngleSnapping();
  switch (p.getLEDState(MouseProfile::SideLED))
  {
    case MouseProfile::LEDOn:
      led_state = LED_STATE_ON;
      break;
    case MouseProfile::LEDOff:
      led_state = LED_STATE_OFF;
      break;
    case MouseProfile::LEDPulsate:
      led_state = LED_STATE_PULSATE;
      break;
    default:
      led_state = 0;
      break;
  }
  settings_buffer[SIDE_LED_STATE_INDEX] = led_state;
  switch (p.getLEDState(MouseProfile::WheelLED))
  {
    case MouseProfile::LEDOn:
      led_state = LED_STATE_ON;
      break;
    case MouseProfile::LEDOff:
      led_state = LED_STATE_OFF;
      break;
    case MouseProfile::LEDPulsate:
      led_state = LED_STATE_PULSATE;
      break;
    default:
      led_state = 0;
      break;
  }
  settings_buffer[WHEEL_LED_STATE_INDEX] = led_state;
  color = p.getLEDColor(MouseProfile::SideLED);
  settings_buffer[SIDE_LED_RED_INDEX] = (color >> 16) & 0xff;
  settings_buffer[SIDE_LED_GREEN_INDEX] = (color >> 8) & 0xff;
  settings_buffer[SIDE_LED_BLUE_INDEX] = color & 0xff;
  color = p.getLEDColor(MouseProfile::WheelLED);
  settings_buffer[WHEEL_LED_RED_INDEX] = (color >> 16) & 0xff;
  settings_buffer[WHEEL_LED_GREEN_INDEX] = (color >> 8) & 0xff;
  settings_buffer[WHEEL_LED_BLUE_INDEX] = color & 0xff;

  for(int i=0;i<BUTTON_COUNT;i++)
  {
    b = p.getButtonSetting(static_cast<MouseProfile::MouseButton>(i));
    switch (b.getType())
    {
      case ButtonSetting::MouseAction:
        button_buffer[i][1] = b.getValue();
        break;
      case ButtonSetting::MediaButton:
        button_buffer[i][0] = 0x04;
        button_buffer[i][3] = b.getValue();
        break;
      case ButtonSetting::CommonShortcut:
        button_buffer[i][0] = 0x55;
        button_buffer[i][3] = b.getValue();
        break;
      case ButtonSetting::SingleKey:
        button_buffer[i][1] = b.getValue();
        break;
      case ButtonSetting::Macro:
        // TODO Check Macro
        if (b.getMacro().size() > button_size)
        {
          setError("Macro too big.");
          return false;
        }
        memcpy(button_buffer[i], &b.getMacro()[0], b.getMacro().size());
        if(i<8)
        {
          settings_buffer[MACRO_BITS_0_7_INDEX] |= 0x01 << i;
        } else {
          settings_buffer[MACRO_BITS_8_11_INDEX] |= 0x01 << (i-8);
        }
        break;
      case ButtonSetting::Disabled:
        break;
    }
  }

  /* Send settings to mouse */
  command_buffer[0]=0x07;
  command_buffer[1]=0x02;
  command_buffer[2]=profile_number;

  // 55 AA 33 CC
  command_buffer[28]=0x55;
  command_buffer[29]=0xaa;
  command_buffer[30]=0x33;
  command_buffer[31]=0xcc;

  bool result = setReport(command_buffer, 32);
  if(!result)
  {
    return false;
  }

  /* Send settings part */
  result = setReport(settings_buffer, 32);

  if(!result)
  {
    return false;
  }

  /* Send buttons part */
  for(int i=0;i<BUTTON_COUNT;i++)
  {
      result = setReport(button_buffer[i], button_size);
      if(!result)
        return false;
  }
  return true;
}

uint16_t MouseSetupInterface::getFirmwareVersion() const
{
  return m_mouse_version;
}

bool MouseSetupInterface::isOpen() const
{
  /* m_handle is set to nullptr if not opened successfully */
  return m_handle;
}

std::string MouseSetupInterface::getLastErrorMessage() const
{
  return m_last_error_message;
}

/**
 * \param size is assumed to be a multiple of 32.
 * \return false on failure. Set error message.
 */
bool MouseSetupInterface::setReport(unsigned char *buffer,
    int size)
{
  for(int i=0;i<size;i+=32)
  {
    ssize_t result = libusb_control_transfer(m_handle,
        LIBUSB_REQUEST_TYPE_CLASS | LIBUSB_RECIPIENT_INTERFACE,
        0x09, // HID class-specific SET_REPORT request
        0x0300, // Report type = 3 (Feature), Report ID = 0
        2, // Interface
        buffer+i,
        32,
        USB_TIMEOUT);
    if (result<0)
    {
      setLibUSBError("Error libusb_control_transfer :", result);
      return false;
    }
  }
  return true;
}

void MouseSetupInterface::setLibUSBError(const char *message,
    int error_code)
{
  m_last_error_message.clear();
  m_last_error_message.append(message);
  m_last_error_message.append(
      libusb_strerror(static_cast<libusb_error>(error_code)));
}

void MouseSetupInterface::setError(const char *message)
{
  m_last_error_message.clear();
  m_last_error_message.append(message);
}

void MouseSetupInterface::releaseAndCloseHandle()
{
  if(m_handle)
  {
    int err = 0;
    err = libusb_release_interface(m_handle, 2);
    if(err)
    {
      setLibUSBError("Error release interface : ", err);
    }
    libusb_close(m_handle);
    m_handle = nullptr;
  }
}
