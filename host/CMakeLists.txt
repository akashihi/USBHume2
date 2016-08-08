#Preamble
cmake_minimum_required(VERSION 2.8)
project(USBHume2)

#Libraries detection
find_package ( PkgConfig )
pkg_check_modules ( PKGCONFIG_LIBUSB libusb )

#Documentation stuff
set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR})
include(UseDoxygen)
set(DOXYFILE_LATEX OFF)

#Build configuration
include_directories(${CMAKE_SOURCE_DIR}/src/lib ${PKGCONFIG_LIBUSB_INCLUDE_DIRS})

add_library(libusbhume STATIC src/lib/libusbhume.c src/lib/libusbhume.h )

add_executable(usbhume src/usbhume/usbhume.c)

target_link_libraries(usbhume libusbhume ${PKGCONFIG_LIBUSB_LIBRARIES})