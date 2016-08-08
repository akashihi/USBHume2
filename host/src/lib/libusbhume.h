#ifndef __LIBUSBHUME_H
#define __LIBUSBHUME_H

#include <stdint.h>

#define USBHUME_VID 0xF055
#define USBHUME_PID 0xA8D1

#define USBHUME_ERR_NOERR 0x00
#define USBHUME_ERR_NORESP 0x01
#define USBHUME_ERR_NOTRANSFER 0x02
#define USBHUME_ERR_CRCERR 0xED
#define USBHUME_ERR_TIMEOUT 0xFF
#define USBHUME_ERR_RESET 0xEE

struct usbhumeDevice {
    void *handle;
};

struct usbhumeData {
    uint8_t rh;
    uint8_t t;
    uint8_t hwErr;
    uint8_t swErr;
};

struct usbhumeDevice* usbhumeOpen();
struct usbhumeData* usbhumeRequest(struct usbhumeDevice*);
void usbhumeClose(struct usbhumeDevice*);


#endif /* __LIBUSBHUME_H */