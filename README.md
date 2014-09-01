func-mouse-config
=================

Configuration tool for the Func MS-3 mouse (firmware version 1.12 and 1.1c) for Linux.

###Building

Require libusb >= 1.0, TinyXML and Qt >= 5.2 for the GUI.

    cd func-mouse-config
    mkdir build
    cd build
    cmake -DCMAKE_INSTALL_PREFIX=/usr ..
    make

And eventually `make install` or just copy `qt_gui/func-config-qt` and
`cli/func-config-cli` where you want. There is *no* make uninstall.

###Usage

Require write permissions to the mouse.
One way to do that is with a udev rule to give access to the user using ACL :

    SUBSYSTEM=="usb", ATTR{idVendor}=="040b", ATTR{idProduct}=="0a68", TAG+="uaccess"


###Using the GUI

- The macro input is pretty crude currently. It displays what will be 
   sent to the mouse, not a human readable list of buttons.
- The single key setting also only displays the HID Usage ID for the key for now.
- I'm not at all a GUI designer, so other things could be really confusing to anyone but me ... :) Suggestions are welcome.

###Using cli :

- Set polling interval : `func-config-cli interval <value>`
 
    `<value>` is in milliseconds (1 = 1000Hz, 2 = 500Hz, 4 = 250Hz, 8 = 125Hz).

- Change active profile : `func-config-cli switch <p_num>`

    Switch to profile `<p_num>` (1,2 or 3).

- Send profile to mouse (from XML file) : `func-config-cli set <p_num> <path>`

    Read profile (from xml file created with the GUI, it's not currently 
    really human-readable) at `<path>` and send to mouse profile `<p_num>`.

- Set active DPI slot for each profile : `func-config-cli dpi <pro1> [pro2] [pro3]`

    Change the selected DPI slot *for all three profiles*. Missing values will be
    equal to the given value.
