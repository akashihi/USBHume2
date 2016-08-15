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

#ifndef __DHT_H
#define __DHT_H

#include "gpio.h"

/**
  * DHT11 single wire byte reading function.
  */
uint8_t readByte(void);

/**
  * DHT11/DHT22 reading procedure. Request-confirmation-response.
  */
uint8_t* readDHT(void);

#endif /* __DHT_H */
