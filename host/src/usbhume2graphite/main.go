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

import (
	"fmt"
	"github.com/marpaia/graphite-golang"
	"strconv"
	"time"
)

func wait(configuration Configuration) {
	time.Sleep(time.Duration(configuration.Period) * time.Second)
}

func main() {
	var configuration = config()
	for {
		t, rh, err := request()
		if err != nil {
			fmt.Printf("Request error: %v", err)
			wait(configuration)
			continue
		}

		Graphite, err := graphite.NewGraphite(configuration.MetricsHost, configuration.MetricsPort)
		if err != nil {
			fmt.Printf("Can't connect to graphite collector: %v", err)
			wait(configuration)
			continue
		}

		Graphite.SimpleSend(fmt.Sprint(configuration.MetricsPrefix, ".T"), strconv.Itoa(int(t)))
		Graphite.SimpleSend(fmt.Sprint(configuration.MetricsPrefix, ".RH"), strconv.Itoa(int(rh)))

		wait(configuration)
	}
}
