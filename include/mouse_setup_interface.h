#ifndef MOUSE_SETUP_INTERFACE_H
#define MOUSE_SETUP_INTERFACE_H

#include "mouse_profile.h"

#include <libusb.h>
#include <string>

class MouseSetupInterface
{
  public:
    MouseSetupInterface();
    ~MouseSetupInterface();
    // No copies
    MouseSetupInterface(const MouseSetupInterface&) = delete;
    MouseSetupInterface& operator=(const MouseSetupInterface&) = delete;
    MouseSetupInterface(MouseSetupInterface&&) = delete;
    MouseSetupInterface& operator=(MouseSetupInterface&&) = delete;

    /**
     * Is called in constructor. Can be called if mouse get disconnected.
     * \return true if a supported mouse is found and ready to be used.
     */
    bool findMouse();

    /**
     * Change the polling interval. This makes the mouse restart and the
     * handle become invalid. findMouse must be called before doing other
     * operations.
     * \param interval in miliseconds ( 1 = 1000Hz, 2 = 500Hz, 4 = 250Hz ...)
     * Warning : will accept any stupid value.
     * \return true on success.
     */
    bool setPollingInterval(unsigned char interval);

    /**
     * Change selected profile to \param profile_number (1, 2 or 3)
     * \return true on success.
     */
    bool switchToProfile(unsigned char profile_number);

    /**
     * Sends the profile settings to the mouse.
     * \param profile_number is the mouse profile slot (1, 2, 3)
     * \return true on success.
     */
    bool setProfile(const MouseProfile &p,
        unsigned char profile_number);

    /**
     * Change selected DPI setting slot for each profile.
     * The 3 values are set in the same SET_REPORT request.
     * \param dpi_profile is 0x01, 0x02 or 0x03
     * \return true on success.
     */
    bool setSelectedDPI(unsigned char dpi_profile1,
        unsigned char dpi_profile2, unsigned char dpi_profile3);

    /**
     * \return Mouse firmware version (bcdDevice)
     */
    uint16_t getFirmwareVersion() const;

    /**
     * \return true if supported mouse was found previously and is ready
     * to use.
     */
    bool isOpen() const;

    /**
     * XXX Use enum error codes instead
     */
    std::string getLastErrorMessage() const;

  private:
    /**
     * Check vendor and product id, and bcdDevice (firmware version)
     * of \param device.
     */
    bool isSupportedMouse(libusb_device *device);

    /**
     * Send \param buffer in HID class SET_REPORT requests to the control
     * endpoint, addressed to interface 2, in chunks of 32 bytes (as it
     * seems to be what the mouse expects ...).
     * \param size should be a multiple of 32
     */
    bool setReport(unsigned char *buffer, int size);

    /**
     * Set last error message to message followed by the string provided by
     * libusb_strerror corresponding to the error_code.
     * \param message : a nul terminated string.
     */
    void setLibUSBError(const char *message, int error_code);

    /**
     * Set \param message as last error message.
     */
    void setError(const char *message);

    /**
     * If m_handle is not null, release interface and call
     * libusb_close on m_handle.
     * Otherwise does nothing.
     */
    void releaseAndCloseHandle();

    libusb_context *m_context;
    libusb_device_handle *m_handle = nullptr;
    uint16_t m_mouse_version; /** bcdDevice : firmware version */
    std::string m_last_error_message;
};
#endif
