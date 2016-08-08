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
	"flag"
)

type Configuration struct {
	Period        int
	MetricsHost   string
	MetricsPort   int
	MetricsPrefix string
}

func config() Configuration {
	periodPtr := flag.Int("period", 60, "Period of status page polling in seconds")
	metricsHostPtr := flag.String("metrics-host", "127.0.0.1", "Ip address of the Graphite collector host")
	metricsPortPtr := flag.Int("metrics-port", 2003, "Port of the Graphite collector host")
	metricsPrefixPtr := flag.String("metrics-prefix", "usbhume2", "Prefix to prepend metrics names")

	flag.Parse()

	return Configuration{Period: *periodPtr, MetricsHost: *metricsHostPtr, MetricsPort: *metricsPortPtr, MetricsPrefix: *metricsPrefixPtr}
}
