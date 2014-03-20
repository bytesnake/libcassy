# libcassy
This is a Cassy Library written in C and based on the official documentation.

Supported functions:
	* AnalogRead (Sensor-Cassy 1/2)
	* Oscilloscope (Sensor-Cassy 1/2)
	* AnalogWrite (Power-Cassy)
	* FunctionGenerator (Power-Cassy)
	* and even more!

## Quick Start
### Linux
1. Make sure you have installed the dependencies:
	* gcc
	* GNU make
	* (libusb)
2. Build:
	You can either use Linux Device Nodes or libusb:
		
	Dev-Nodes:
		$ make 
	libusb:
		$ make libusb
3. Use it:
	There are some examples in test/, read and learn them!

### Windows
1. Download and install MinGW
2. Build:
	Windows is at the moment only supported by libusb:
		$ make libusb
3. See above

## License
Libcassy is distributed under the terms of the LGPL license (version 3.0)

See LICENSE
