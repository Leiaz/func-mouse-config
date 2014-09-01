# FindLibUSB1 
#
# Finds libusb-1.0 

# This will define the following variables :
# LibUSB1_FOUND if is found
# LibUSB1_INCLUDE_DIRS the include directories.
# LibUSB1_LIBRARIES the libraries to link against.

find_package(PkgConfig)
pkg_check_modules(PC_LibUSB1 QUIET libusb-1.0)

find_path(LibUSB1_INCLUDE_DIR
  NAMES libusb.h
  PATHS ${PC_LibUSB1_INCLUDE_DIRS}
)
find_library(LibUSB1_LIBRARY
  NAMES usb-1.0
  PATHS ${PC_LibUSB1_LIBRARY_DIRS}
)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(LibUSB1
  FOUND_VAR LibUSB1_FOUND
  REQUIRED_VARS
    LibUSB1_LIBRARY
    LibUSB1_INCLUDE_DIR
)

if(LibUSB1_FOUND)
  set(LibUSB1_LIBRARIES ${LibUSB1_LIBRARY})
  set(LibUSB1_INCLUDE_DIRS ${LibUSB1_INCLUDE_DIR})
endif()

mark_as_advanced(LibUSB1_INCLUDE_DIR
                 LibUSB1_LIBRARY)
