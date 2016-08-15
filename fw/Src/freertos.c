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

#include "FreeRTOS.h"
#include "task.h"
#include "cmsis_os.h"
#include "gpio.h"
#include "usb_device.h"
#include "usbd_customhid.h"
#include "dht.h"

osThreadId wdgTaskHandle;
osThreadId dhtTaskHandle;
osThreadId usbTaskHandle;
osMessageQId usbQueueHandle;
osSemaphoreId wdgSemHandle;
osSemaphoreId dhtSemHandle;

extern TIM_HandleTypeDef htim2;
extern TIM_HandleTypeDef htim3;

void StartWdgTask(void const * argument);
void StartDhtTask(void const * argument);
void StartUsbTask(void const * argument);

extern void MX_USB_DEVICE_Init(void);
void MX_FREERTOS_Init(void); 

void MX_FREERTOS_Init(void) {
  osSemaphoreDef(wdgSem);
  wdgSemHandle = osSemaphoreCreate(osSemaphore(wdgSem), 1);

  osSemaphoreDef(dhtSem);
  dhtSemHandle = osSemaphoreCreate(osSemaphore(dhtSem), 1);

  osThreadDef(wdgTask, StartWdgTask, osPriorityNormal, 0, 128);
  wdgTaskHandle = osThreadCreate(osThread(wdgTask), NULL);

  osThreadDef(dhtTask, StartDhtTask, osPriorityNormal, 0, 128);
  dhtTaskHandle = osThreadCreate(osThread(dhtTask), NULL);

  osThreadDef(usbTask, StartUsbTask, osPriorityIdle, 0, 128);
  usbTaskHandle = osThreadCreate(osThread(usbTask), NULL);

  osMessageQDef(usbQueue, 2, uint16_t);
  usbQueueHandle = osMessageCreate(osMessageQ(usbQueue), NULL);
}

void StartWdgTask(void const * argument) {
	char state = 1;
	HAL_TIM_Base_Start_IT(&htim3);
  for(;;) {
		if (state) {
			state = 0;
			HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, GPIO_PIN_SET);
		} else {
			state = 1;
			HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, GPIO_PIN_RESET);
		}
		xSemaphoreTake(wdgSemHandle, portMAX_DELAY);
  }
}

void StartDhtTask(void const * argument) {
  HAL_Delay(4000); //We need to give DHT11 couple of seconds to settle.
	
	HAL_TIM_Base_Start_IT(&htim2);
  for(;;) {
    xQueueSend(usbQueueHandle, readDHT(),0);
		xSemaphoreTake(dhtSemHandle, 120000/portTICK_PERIOD_MS);
  }
}

void StartUsbTask(void const * argument) {
	uint8_t usbData[4];
	
	usbData[0] = 0;
	usbData[1] = 0;
	usbData[2] = 0;
	usbData[3] = 0;

  MX_USB_DEVICE_Init();
  for(;;) {
 		xQueueReceive(usbQueueHandle, &usbData, 10/portTICK_PERIOD_MS);		
		USBD_CUSTOM_HID_SendReport(&hUsbDeviceFS, usbData, 4);
  }
}
