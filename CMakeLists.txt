cmake_minimum_required (VERSION 2.8)
project(func-mouse-config)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${PROJECT_SOURCE_DIR}/cmake/")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall -pedantic")

find_package(LibUSB1 REQUIRED)
include_directories(SYSTEM ${LibUSB1_INCLUDE_DIRS})

find_package(TinyXML REQUIRED)
include_directories(SYSTEM ${TinyXML_INCLUDE_DIRS})

include_directories("include")

add_subdirectory(common)
add_subdirectory(cli)
add_subdirectory(qt_gui)
