CC=gcc
CFLAGS=-x c -shared -Wall -fPIC -DCA_LOCAL
LDFLAGS=-ldl
SOURCES=$(shell find source -type d -name ioplatforms -prune -o -name "*.c" -print | xargs)
OUTPUT=binary/libcassy.so

default:
	$(CC) -g $(CFLAGS) -DCA_DEVNODES $(SOURCES) source/ioplatforms/ca_inout_devnodes.c $(LDFLAGS) -o $(OUTPUT)
release:
	$(CC) -O3 $(CFLAGS) -DCA_DEVNODES $(SOURCES) source/ioplatforms/ca_inout_devnodes.c $(LDFLAGS) -o $(OUTPUT)
debug:
	$(CC) -g $(CFLAGS) -DCA_DEVNODES -DCA_DEBUG_PRINT $(SOURCES) source/ioplatforms/ca_inout_devnodes.c $(LDFLAGS) -o $(OUTPUT)
nowrite:
	$(CC) -g $(CFLAGS) -DCA_DEVNODES -DCA_DEBUG_NOWRITE $(SOURCES) source/ioplatforms/ca_inout_devnodes.c $(LDFLAGS) -o $(OUTPUT)
noread:
	$(CC) -g $(CFLAGS) -DCA_DEVNODES -DCA_DEBUG_NOREAD $(SOURCES) source/ioplatforms/ca_inout_devnodes.c $(LDFLAGS) -o $(OUTPUT)
libusb:
	$(CC) -g $(CFLAGS) -I/usr/include/libusb-1.0 -DCA_LIBUSB $(SOURCES) source/ioplatforms/ca_inout_libusb.c $(LDFLAGS) -lusb-1.0 -o $(OUTPUT) 
clean:
	rm -f $(OUTPUT)
