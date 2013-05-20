CC=gcc
SOURCES=$(shell find source -type d -name ioplatforms -prune -o -name "*.c" -print | xargs)

ifeq ($(OS),Windows_NT)
	CFLAGS=-x c -shared -Wall -DCA_LOCAL
	LDFLAGS=
	DEFAULTPRE=-DCA_WINHID
	DEFAULTSRC=source/ioplatforms/ca_inout_winhid.c
	OUTPUT=binary/libcassy.dll
else
	CFLAGS=-x c -shared -Wall -fPIC -DCA_LOCAL
	LDFLAGS=-ldl
	DEFAULTPRE=-DCA_DEVNODES
	DEFAULTSRC=source/ioplatforms/ca_inout_devnodes.c
	OUTPUT=binary/libcassy.so
endif

default:
	$(CC) -g $(CFLAGS) $(DEFAULTPRE) $(SOURCES) $(DEFAULTSRC) $(LDFLAGS) -o $(OUTPUT)
release:
	$(CC) -O3 $(CFLAGS) $(DEFAULTPRE) $(SOURCES) $(DEFAULTSRC) $(LDFLAGS) -o $(OUTPUT)
debug:
	$(CC) -g $(CFLAGS) $(DEFAULTPRE) -DCA_DEBUG_PRINT $(SOURCES) $(DEFAULTSRC) $(LDFLAGS) -o $(OUTPUT)
devnodes:
	$(CC) -g $(CFLAGS) -DCA_DEVNODES $(SOURCES) source/ioplatforms/ca_inout_devnodes.c $(LDFLAGS) -o $(OUTPUT)
libusb:
	$(CC) -g $(CFLAGS) -I/usr/include/libusb-1.0 -DCA_LIBUSB $(SOURCES) source/ioplatforms/ca_inout_libusb.c $(LDFLAGS) -lusb-1.0 -o $(OUTPUT) 
winhid:
	$(CC) -g $(CFLAGS) -DCA_WINHID $(SOURCES) source/ioplatforms/ca_inout_winhid.c $(LDFLAGS) -o $(OUTPUT)
clean:
	rm -f $(OUTPUT)
