CC=gcc
CFLAGS=-g -I../../include -DCA_DEVNODES
LDFLAGS=-L../../binary -lcassy -Wl,-rpath=../../binary

clean:
	rm -f checkleds checkmemory relay squaresrc
autoscanning:
	$(CC) -g -I../../include -DCA_LIBUSB autoscanning.c -L../../binary -lcassy -lusb-1.0 -Wl,-rpath=../../binary
%:
	$(CC) $(CFLAGS) $@.c $(LDFLAGS) -o $@
