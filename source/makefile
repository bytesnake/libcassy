CC=gcc
CFLAGS=-x c -shared -Wall -fPIC -DCA_LOCAL
LDFLAGS=-lm -ldl
SOURCES=ca_misc.c ca_error.c ca_data.c ca_stream.c ca_inout.c functions/ca_utility.c functions/ca_analog.c functions/ca_relay.c functions/ca_digital.c functions/ca_oscilloscope.c
OUTPUT=../binary/libcassy.so

default:
	$(CC) -g $(CFLAGS) -DCA_DEVNODES $(SOURCES) ca_inout_devnodes.c $(LDFLAGS) -o $(OUTPUT)
release:
	$(CC) -O3 $(CFLAGS) -DCA_DEVNODES $(SOURCES) ca_inout_devnodes.c $(LDFLAGS) -o $(OUTPUT)
debug:
	$(CC) -g $(CFLAGS) -DCA_DEVNODES -DCA_DEBUG_PRINT $(SOURCES) ca_inout_devnodes.c $(LDFLAGS) -o $(OUTPUT)
nowrite:
	$(CC) -g $(CFLAGS) -DCA_DEVNODES -DCA_DEBUG_NOWRITE $(SOURCES) ca_inout_devnodes.c $(LDFLAGS) -o $(OUTPUT)
noread:
	$(CC) -g $(CFLAGS) -DCA_DEVNODES -DCA_DEBUG_NOREAD $(SOURCES) ca_inout_devnodes.c $(LDFLAGS) -o $(OUTPUT)
libusb:
	$(CC) -g $(CFLAGS) -I/usr/include/libusb-1.0 -DCA_LIBUSB $(SOURCES) ca_inout_libusb.c $(LDFLAGS) -lusb-1.0 -o $(OUTPUT) 
clear:
	rm -f $(OUTPUT)
