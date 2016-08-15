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
#include "dht.h"

uint8_t readByte() {
    int8_t indx;
    uint8_t data=0x00;
		int delay;
	
    for (indx=7;indx>-1;indx--) {
	delay=10000;//Wait for confirmation
	while (HAL_GPIO_ReadPin(DHT22_GPIO_Port, DHT22_Pin) != GPIO_PIN_RESET) {
	    if (delay-- == 0 ) {					
	       return 0xFF;
	    }
        }		
        delay=10000;//Wait for confirmation
        while (HAL_GPIO_ReadPin(DHT22_GPIO_Port, DHT22_Pin) != GPIO_PIN_SET) {
	    if (delay-- == 0 ) {
                return 0xEE;
	    }
        }		
        delay = 50;
        while (--delay >0 )
        if (HAL_GPIO_ReadPin(DHT22_GPIO_Port, DHT22_Pin) == GPIO_PIN_SET) {
            data |= (1 << (indx));
        } else {
	    data &= ~(1 << (indx));
        }
    }

    return data;
}

uint8_t* readDHT(void) {
    static uint8_t dataToSend[4];
    int delay;
	
    dataToSend[0]=0;
    dataToSend[1]=0;
    dataToSend[2]=0;
    dataToSend[3]=0;
	
    GPIO_InitTypeDef GPIO_InitStruct;

    //Initiate request
    HAL_GPIO_WritePin(DHT22_GPIO_Port, DHT22_Pin, GPIO_PIN_RESET); //Set to low
    GPIO_InitStruct.Pin = DHT22_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(DHT22_GPIO_Port, &GPIO_InitStruct);
		HAL_Delay(20); //Wait for 20 ms until DHT22 is ready
		taskENTER_CRITICAL(); //Starting from that moment will only read bus
    GPIO_InitStruct.Pin = DHT22_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(DHT22_GPIO_Port, &GPIO_InitStruct); //Release bus
    delay=10000;//Wait for confirmation
    while (HAL_GPIO_ReadPin(DHT22_GPIO_Port, DHT22_Pin) != GPIO_PIN_RESET) {
        if (delay-- == 0 ) {
	    dataToSend[1]=1;
	    taskEXIT_CRITICAL();
	    return dataToSend;
	}
    }
    delay=10000;//Wait for transfer start
    while (HAL_GPIO_ReadPin(DHT22_GPIO_Port, DHT22_Pin) != GPIO_PIN_SET) {
        if (delay-- == 0 ) {
	    dataToSend[1]=2;
	    taskEXIT_CRITICAL();
	    return dataToSend;
	}
    }
		
    dataToSend[0]=readByte();
    readByte(); //Skip empty byte
    dataToSend[2]=readByte();
    readByte(); //Skip empty byte
    uint8_t crc = readByte();
		
    taskEXIT_CRITICAL();

    if (crc != dataToSend[0]+dataToSend[2]) {
        dataToSend[1]=0xED;
    }

    return dataToSend;
}
