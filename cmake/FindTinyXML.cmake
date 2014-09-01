# FindTinyXML 
#
# Finds tinyxml 

# This will define the following variables :
# TinyXML_FOUND if is found
# TinyXML_INCLUDE_DIRS the include directories.
# TinyXML_LIBRARIES the libraries to link against.

find_package(PkgConfig)
pkg_check_modules(PC_TinyXML QUIET tinyxml)

find_path(TinyXML_INCLUDE_DIR
  NAMES tinyxml.h
  PATHS ${PC_TinyXML_INCLUDE_DIRS}
)
find_library(TinyXML_LIBRARY
  NAMES tinyxml
  PATHS ${PC_TinyXML_LIBRARY_DIRS}
)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(TinyXML
  FOUND_VAR TinyXML_FOUND
  REQUIRED_VARS
    TinyXML_LIBRARY
    TinyXML_INCLUDE_DIR
)

if(TinyXML_FOUND)
  set(TinyXML_LIBRARIES ${TinyXML_LIBRARY})
  set(TinyXML_INCLUDE_DIRS ${TinyXML_INCLUDE_DIR})
endif()

mark_as_advanced(TinyXML_INCLUDE_DIR 
                 TinyXML_LIBRARY)
