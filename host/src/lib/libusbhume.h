/*
   This file is a part of USBHume2.
   Copyright (C) 2016 Denis V Chapligin <akashihi@gmail.com>
   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
/** @file */
/*! \mainpage libusbhume - interface for USB hygrometer USBHume2.
     
    \section Introduction
     
     USBHume2 device is a USB HID class device, so it soesn't requires special drivers. 

     Device can only be read and it usually returns relative humidity in percents and  temperature in Celcius degrees or error in case
     of sensor failure. 

     libusbhume is mostly a wrapper over libusb, that hides low level stuff and allows end user to work with high level abstractions. 
     At the moment it doesn't support neither several devices nor device selection and it's functionality is very limited.

     libusbhume usage is quite easy:
     \li Open device
     \li Request measurements one or more times
     \li Close device 
    
     \section GaI Getting and installing
     
     The libusbhume source code is hosted on <a href="https://github.com/akashihi/USBHume2">github</a>, as a partof USBHume2 repository, so you can download the source code
     using wget or other download tool:
     
     \li https://github.com/akashihi/USBHume2/tarball/master (tar.gz format)
     \li https://github.com/akashihi/USBHume2/zipball/master (or zip format)
     
     You could also clone the repository and check some unstable code:
     
     \code
     git clone git://github.com/akashihi/USBHume2.git
     \endcode
     
     The 'master' branch is always pointing to the latest stable release and the 'next' branch is a unstable development branch.
     
     Host part of USBHume2 (libusbhume and some examples) is in /host directory.  To build libusbhume you'll need:
     
     \li <a href="http://www.cmake.org/">Recent CMake (required)</a>
     \li <a href="http://libusb.org//">libusb (required)</a>
     \li <a href="http://www.doxygen.org/">Doxygen (optional, only for documentation processing)</a>
     
     After getting this stuff up and running, select a directory for building and issue the following commands:
     
     \code
        cmake /path/to/the/USBHume2/sources/host        
        make
        make test
        make doc
        make install
    \endcode
     
     The CMake is using a <a href="http://www.cmake.org/Wiki/CMake_FAQ#Out-of-source_build_trees">out-of-source</a> build approach,
     so it is recommended to create a temporary build directory somewhere and remove it after installation.
     
     The generated documentation will be put into the \<build\>/doc directory. The documentation is NOT
     installed by 'make install' command, so you have to copy it manually, if you really need it.
     
     \section Usage
     
     Open device, query device, close device.
     
     \code
     struct usbhumeDevice* uh = usbhumeOpen();
     struct usbhumeData *reply = usbhumeRequest(uh);
     free(reply);
     usbhumeClose(uh);
     \endcode
     

     \link struct usbhumeDevice \endlink is a opaque object, whose lifecycle is managed by libusb. On the other hand \link struct usbhumeData \endlink 
     should be freed by your app.

     \section AaL  Authors and Licensing
     
     Copyright (C) 2013 Denis V Chapligin
     Distributed under the GNU General Public License, Version 3.
     (See accompanying file LICENSE)
     
*/
#ifndef __LIBUSBHUME_H
#define __LIBUSBHUME_H

#include <stdint.h>

/*!
 * \brief USBHume2 vendor id. We use 0xF055 as VID.
 */
#define USBHUME_VID 0xF055
/*!
 * \brief USBHume2 device id. Randomly generated :)
 */
#define USBHUME_PID 0xA8D1

/*!
 * \brief No error happened during conversation with sensor.
 * Any other value usually means hardware malfunction. 
 *
 * It could be, that sensor acts incorrectly during first
 * several seconds after being powered on, but if error is
 * repeatable, it means that hardware may need
 * to be replaced.
 */
#define USBHUME_ERR_NOERR 0x00
/*!
 * \brief Sensor didn't replied to request.
 */
#define USBHUME_ERR_NORESP 0x01
/*!
 * \brief Sensor didn't confirmed start of transfer after request confirmation.
 */
#define USBHUME_ERR_NOTRANSFER 0x02
/*!
 * \brief Sensor replied correctly, but CRC check of received data failed.
 * This error means, that received data shouldn't be used.
 * 
 * It is okay to get this error sometime, but if it repeats continuosly, it means
 * you have to replate your USBHume2 device.
 */
#define USBHUME_ERR_CRCERR 0xED
/*!
 * \brief Next transfer of a bit didn't started during expected transfer time.
 */
#define USBHUME_ERR_TIMEOUT 0xFF
/*!
 * \brief Timeout happened waiiting for actual bit to arrive.
 */
#define USBHUME_ERR_RESET 0xEE

/*!
 * \struct usbhumeDevice 
   \brief USBHume2 device descriptor.
 * Opaque structure with lifecycle managed by libusbhume.
 * \see usbhumeOpen
 * \see usbhumeClose
 */
struct usbhumeDevice {
    void *handle;
};

/*!
 * \struct usbhumeData
 * \brief USBHume2 measurement response data.
 *
 * Don't forget, you have to free that structure manually.
 */
struct usbhumeData {
    /*!
     * \brief Relative humidity in percents.
     */
    uint8_t rh;
    /*!
     * \brief Temperature in Celsius degrees.
     */
    uint8_t t;
    /*!
     * \brief Hardware error value. If it is not 0, data shouldn't be used. Check USBHUME_ERR_* for details.
     */
    uint8_t hwErr; 
    /*!
     * \brief Host software error. Actually a libusb error. If it is less then 0, data shouldn't be used.
     */
    uint8_t swErr;
};

/*!
 * \fn struct usbhumeDevice* usbhumeOpen();
 * \brief Tries to open USBHume2 device.
 * \return USBHume2 device handle or NULL in case of error or when no device is attached. 
 * returned device handle should be cleared with \see usbhumeClose.
 */
struct usbhumeDevice* usbhumeOpen();

/*!
 * \fn struct usbhumeData* usbhumeRequest(struct usbhumeDevice* uh);
 * \brief Requests measurement data from the USBHume2 device.
 * \param uh handle of USBHume2 device, returned by \see usbhumeOpen.
 * \return \see usbhumeData pointer with measurement data. You have to
 * free that pointer manually. 
 * This function always returns pointer to \see usbhumeData and
 * marks error condition within fields swErr and hwErr. You 
 * need to check them prioir using measurement data.
 */
struct usbhumeData* usbhumeRequest(struct usbhumeDevice* uh);

/*!
 * \fn void usbhumeClose(struct usbhumeDevice* uh);
 * \brief Closes USBHume2 device handler and releases all resources.
 * \para uh Device handler returned by \see usbhumeOpen
 */
void usbhumeClose(struct usbhumeDevice* uh);


#endif /* __LIBUSBHUME_H */