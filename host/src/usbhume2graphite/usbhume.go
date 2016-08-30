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

package main

/*
#cgo LDFLAGS: -L. -lusbhume -lusb
#include "libusbhume.h"

*/
import "C"

import (
	"errors"
)

type UsbHume struct {
	device *C.struct_usbhumeDevice
}

func openUsbHume() (*UsbHume, error) {
	handle := C.usbhumeOpen()
	if handle == nil {
		return nil, errors.New("Can't open device")
	}
	return &UsbHume{
		device: handle,
	},nil
}

func (d *UsbHume) request() (C.uint8_t, C.uint8_t, error) {
	result := C.usbhumeRequest(d.device)

	if result.swErr < 0 {
		return 0, 0, errors.New("libusb error")
	}
	if result.hwErr != 0 {
		return 0, 0, errors.New("Hardware error")
	}

	return result.t, result.rh, nil
}

func (d *UsbHume) closeUsbHume() {
	C.usbhumeClose(d.device)
}
