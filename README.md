# USBHume2

USB hygrometer and thermometer, based on DTH11/DHT22 sensor.

Once i needed to log humidity change on the premises and it was faster, to build usb based sensor myself, rather then to order ready made device and wait several weeks for delivery

## Getting Started

The solution consists of three parts, combined in a [single repository](https://github.com/akashihi/USBHume2) at github. Those parts are:

* Schematics and board
* Firmware
* Host software

### Prerequisities

For host software you'll need [libusb](http://libusb.org/), [CMake](https://cmake.org/) and, obviously, C compiler. Host software also includes some optional code in Go, so you may need a [Go development kit](https://golang.org/)

Firmware is built on to of STM32 HAL, FreeRTOS and ST's USB library. You will need [Keil uVision 5](http://www.keil.com/) to build or modify it. Pre-built copy of firmware is included into each release.

Schematics and board made with [Eagle](https://cadsoft.io/). PCB is single layer and is compatible with [IDE24S enclosure](http://rcl.lt/index.php?id=2&lang=en&begin=0&acc=show&fid=563&pcode=IDE24S&pmode=&frame=3). Release kit includes PDF with copper, cream and silk masks.

### Making the device

* Make a PCB using your favourite process or order it somewhere.
* Solder USB connector and power supply part (IC1, C3, C4, C5, C7, JP1)
* Plug device into USB and check voltage between JP1 pins, it should be 3.3V
* Solder everything else

#### Partlist

* 1x STM32F042F6P6
* 1x LM1117IMP-3.3
* 2x 10u tantalum capacitors (C5, C7)
* 5x 100n 1206 capacitors (C1, C2, C3, C4, C6)
* 1x 510 0805 resistor (R1)
* 1x 10k 0805 resistor (R2)
* 1x 1k 0805 resistor (R4)
* 1x chip led 0805 (LED3)

### Making the firmware

Install [Keil uVision 5](http://www.keil.com/) and build firmware from directory **fw** using normal procedure.

#### Firmware implementation details

Firmware is built on top of STM32 HAL, FreeRTOS and ST's USB library. In implements custom HID device, that can be only read.

**VID: 0xF055**

**PID: 0xA8D1**

Firmware provides out interface 0 with report id 0x81. Requesting that report will result in getting four bytes:

* humidity value
* error code
* temperature value
* always zero

If error code is not zero, other values shouldn't be trusted. Possible values for error code are:

* 0x01 - sensor doesn't replies to requests.
* 0x02 - sensor confirmed request, but didn't transferred response.
* 0xED - request transferred ok, but CRC check failed.

It is normal to see those errors from time to time (especially after device power-up), but continuous stream of errors mean hardware malfunction.

Humidity and temperature values could contain special codes, like 0xFF or 0xEE, meaning timeout during reading that value. Appearance of such codes is usually accompanied with 0xED value in error code field.

Firmware internally is built using three tasks - one task for blinking RDY led to signal that we are still alive and running. Second thread is requesting measurements from sensor every 60 seconds and third thread is responding to usb read request every 100ms or faster.

#### Firmware flashing

After you built your USBHume2 device and built firmware (or downloaded ready made firmware from the release package), you need to flash the firmware into device.

Device uses SWD interface with three pins on the PCB.

* JP1 - SWDIO
* JP2 - SWDCLK
* JP3 - NRST

Connect those pins to your SWD flasher, optionally connect pins JP4 and JP5 to power-on the device and upload the firmware using your flasher's software.

## Host software

Host software consists of abstraction library and couple of usage examples.

### libusbhume - interface for USB hygrometer USBHume2.

#### Introduction

USBHume2 device is a USB HID class device, so it soesn't requires special drivers. 

Device can only be read and it usually returns relative humidity in percents and  temperature in Celcius degrees or error in case of sensor failure. 

libusbhume is mostly a wrapper over libusb, that hides low level stuff and allows end user to work with high level abstractions. At the moment it doesn't support neither several devices nor device selection and it's functionality is very limited.

libusbhume usage is quite easy:
* Open device
* Request measurements one or more times
* Close device 
    
#### Getting and installing
   
The libusbhume source code is hosted on [github](https://github.com/akashihi/USBHume2"), as a partof USBHume2 repository, so you can download the source code using wget or other download tool:
     
* https://github.com/akashihi/USBHume2/tarball/master (tar.gz format)
* https://github.com/akashihi/USBHume2/zipball/master (or zip format)
     
You could also clone the repository and check some unstable code:
     
```
git clone git://github.com/akashihi/USBHume2.git
```
     
The 'master' branch is always pointing to the latest stable release and the 'next' branch is a unstable development branch.
     
Host part of USBHume2 (libusbhume and some examples) is in /host directory.  To build libusbhume you'll need:
     
* [Recent CMake (required)](http://www.cmake.org/)
* [libusb (required)](http://libusb.org/)
* [Doxygen (optional, only for documentation processing)](http://www.doxygen.org/)
     
After getting this stuff up and running, select a directory for building and issue the following commands:
     
```
cmake /path/to/the/USBHume2/sources/host        
make
make test
make doc
make install
```
     
The CMake is using a [out-of-source](http://www.cmake.org/Wiki/CMake_FAQ#Out-of-source_build_trees) build approach, so it is recommended to create a temporary build directory somewhere and remove it after installation.
     
The generated documentation will be put into the *build*/doc directory. The documentation is NOT installed by ```make install``` command, so you have to copy it manually, if you really need it.
     
#### Usage
     
Open device, query device, close device.
     
```c
struct usbhumeDevice* uh = usbhumeOpen();
struct usbhumeData *reply = usbhumeRequest(uh);
free(reply);
usbhumeClose(uh);
```

struct usbhumeDevice is a opaque object, whose lifecycle is managed by libusb. On the other hand struct usbhumeData should be freed by your app.

    
### usbhumereq - measurement request utility

usbhumereq, located in host/src/usbhume queries device for current measurement and prints it to stdout. It's main purpose is to show how to use libusbhume and check that device is working. You need to provide that binary with capabilities to work with usb bus and detach devices from kernel drivers.

```
sudo ./usbhumereq
RH 44%, T 21C
```

### usbhume2graphite - graphite metrics gatherintg utility

usbhume2graphite is a tool to periodically poll device and send humidity/temperature data to the [Graphite](https://graphiteapp.org/).

This tool is built in Go language and requires Go development kit to be installed.

#### How to build usbhume2graphite

* Install [Go](https://golang.org/)
* Install graphite package for Go ```go get -u github.com/marpaia/graphite-golang```
* Build libusbhume, as described above
* Copy libusbhume binary AND header file into usbhume2graphite folder
* Build the binary ```go build```

#### How to use usbhume2graphite

```
sudo ./usbhume2grpahite -period 60 -metrics-host 127.0.0.1 -metrics-port 2003 -metrics-prefix usbhume2
```

Values, shown above, are default values for configuration options.

* *period* defines poll period in seconds. Don't forget, device makes measurements every 60 seconds, so there is no need to poll it more frequently.
* *metrics-host* address of Graphite metrics collector
* *metrics-port* port of Graphite metrics collector
* *metrics-prefix* usbhume2graphite posts two metrics: T and RH and those metrics will be prefixed with *metrics-prefix*.

#Limitations

Measurement limitations are defined by used sensor. For DHT11 it would be:

* RH range: 20%-95%
* RH measurement error: 5%
* Temperature range: 0-50 Celsius degrees
* Temperature measurement error: 2%

As for device itself, it have two limitations:

* Sensor is polled once per 60 seconds. I discovered, that faster polling could heat-up the sensor itself and add more error to measurements. Anyway, under normal conditions temperature and humidity are not changing very fast.
* You can't attach several devices to a single host and library doesn't supports it. As device problem could be fixed by changing serial no in the firmware and keeping it unique over several devices, library would require more changes. May be sometime i'll fix it.

#Bugs

Lots.

# Authors

* **Denis Chapligin** - *Initial work* - https://github.com/akashihi

# License

This project is licensed under the GPLv3 License - see the [LICENSE](LICENSE) file for details.

Firmware code includes parts of FreeRTOS and ST libraries, licensed undet MIT license.
