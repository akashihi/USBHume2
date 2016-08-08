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
        return 1;
    }

    printf("RH: %u%%, T: %uC\n", reply->rh, reply->t);
    
    free(reply);
    usbhumeClose(uh);
    
    return 0;
}