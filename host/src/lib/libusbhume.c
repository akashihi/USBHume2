#include <usb.h>
#include <stdlib.h>

#include "libusbhume.h"

struct usbhumeDevice* usbhumeOpen() {
    usb_dev_handle *USB_Device = NULL;
    struct usb_device *d;
    struct usb_bus *usbBus;

    usb_init();
    usb_find_busses();
    usb_find_devices();

    for (usbBus = usb_get_busses(); usbBus; usbBus = usbBus->next) {
      for (d = usbBus->devices; d; d = d->next) {
          if ((d->descriptor.idVendor == USBHUME_VID) && (d->descriptor.idProduct == USBHUME_PID)) {
              USB_Device = usb_open(d);
              if (usb_detach_kernel_driver_np(USB_Device, 0)<0) {
                  return NULL;
              }
              if (usb_set_configuration(USB_Device, 1)<0) {
                  return NULL;
              }
              if (usb_claim_interface(USB_Device, 0)<0) {
                  return NULL;
              }

              struct usbhumeDevice* result = malloc(sizeof(struct usbhumeDevice));
              result->handle=USB_Device;
              return result;
          }
      }
    }
    return NULL;
}


struct usbhumeData* usbhumeRequest(struct usbhumeDevice* uh) {
    char usbData[64];
    struct usbhumeData* result = malloc(sizeof(struct usbhumeData));

    int ret = usb_bulk_read(uh->handle, 0x81, usbData, 4, 120000);
    if (ret<0) {
        result->swErr = ret;
    } else {
        result->rh = usbData[0];
        result->t=usbData[2];
        result->hwErr=usbData[1];
    }
 
    return result;
}

void usbhumeClose(struct usbhumeDevice* uh) {
    usb_close(uh->handle);
}
