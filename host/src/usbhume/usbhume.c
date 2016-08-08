#include <stdio.h>
#include <stdlib.h>

#include <libusbhume.h>

int main( int argc, char **argv) {
    struct usbhumeDevice* uh = usbhumeOpen();
    if (uh==NULL) {
	fprintf(stderr, "Unable to find device\n");
	return 1;
    }

    struct usbhumeData *reply = usbhumeRequest(uh);
    if (reply->swErr < 0) {
	fprintf(stderr, "Unable to read from device: %d\n", reply->swErr);
        free(reply);
        usbhumeClose(uh);
        return 1;
    }
    if (reply->hwErr != USBHUME_ERR_NOERR ) {
        switch (reply->hwErr) {
        case USBHUME_ERR_NORESP:
            fprintf(stderr,"Sensor not responding\n");
            break;
        case USBHUME_ERR_NOTRANSFER:
            fprintf(stderr,"Sensor not sending data\n");
            break;
        case USBHUME_ERR_CRCERR:
            fprintf(stderr,"Sensor read error\n");
            break;
        case USBHUME_ERR_TIMEOUT:
            fprintf(stderr,"Sensor bit reading timeout\n");
            break;
        case USBHUME_ERR_RESET:
            fprintf(stderr,"Sensor connection reset during bit reading\n");
            break;
        }
        free(reply);
        usbhumeClose(uh);
    }

    printf("RH: %u%%, T: %uC", reply->rh, reply->t);
    
    free(reply);
    usbhumeClose(uh);
    
    return 0;
}