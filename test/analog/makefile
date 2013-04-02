CC=gcc
CFLAGS=-g -I../../include -DCA_LIBUSB
LDFLAGS=-L../../binary -lcassy -lusb-1.0 -ldl -Wl,-rpath=../../binary

clean:
	rm -f plotdata simpleinput
%:
	$(CC) $(CFLAGS) $@.c $(LDFLAGS) -o $@
