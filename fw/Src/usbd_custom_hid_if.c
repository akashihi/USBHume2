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


#include "usbd_custom_hid_if.h"

__ALIGN_BEGIN static uint8_t CUSTOM_HID_ReportDesc_FS[USBD_CUSTOM_HID_REPORT_DESC_SIZE] __ALIGN_END =
{
  /* USER CODE BEGIN 0 */ 
    0x06, 0x00, 0xff,              // 	USAGE_PAGE (Generic Desktop)
    0x09, 0x01,                    // 	USAGE (Vendor Usage 1)
    // System Parameters
    0xa1, 0x01,                    // 	COLLECTION (Application)
    0x85, 0x01,                    //   REPORT_ID (1)
    0x09, 0x01,                    //   USAGE (Vendor Usage 1)
    0x15, 0x00,                    //   LOGICAL_MINIMUM (0)
    0x25, 0x01,                    //   LOGICAL_MAXIMUM (1)
    0x75, 0x08,                    //   REPORT_SIZE (8)
    0x95, 4, 	                   //   REPORT_COUNT (4)
    0xb1, 0x82,                    //   FEATURE (Data,Var,Abs,Vol)
    0x85, 0x01,                    //   REPORT_ID (1)
    0x09, 0x01,                    //   USAGE (Vendor Usage 1)
    0x91, 0x82,                    //   OUTPUT (Data,Var,Abs,Vol)
 
    0x85, 0x02,                    //   REPORT_ID (4)
    0x09, 0x02,                    //   USAGE (Vendor Usage 4)
    0x75, 0x08,                    //   REPORT_SIZE (8)
    0x95, 4, 	                   //   REPORT_COUNT (4)
    0x81, 0x82,                    //   INPUT (Data,Var,Abs,Vol)
   /* USER CODE END 0 */ 
  0xC0    
	/*     END_COLLECTION	             */
}; 

  extern USBD_HandleTypeDef hUsbDeviceFS;

static int8_t CUSTOM_HID_Init_FS     (void);
static int8_t CUSTOM_HID_DeInit_FS   (void);
static int8_t CUSTOM_HID_OutEvent_FS (uint8_t event_idx, uint8_t state);
 

USBD_CUSTOM_HID_ItfTypeDef USBD_CustomHID_fops_FS = 
{
  CUSTOM_HID_ReportDesc_FS,
  CUSTOM_HID_Init_FS,
  CUSTOM_HID_DeInit_FS,
  CUSTOM_HID_OutEvent_FS,
};

static int8_t CUSTOM_HID_Init_FS(void) { 
  return (0);
}

static int8_t CUSTOM_HID_DeInit_FS(void) {
  return (0);
}

static int8_t CUSTOM_HID_OutEvent_FS  (uint8_t event_idx, uint8_t state) { 
  return (0);
}
