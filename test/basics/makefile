CC=gcc
CFLAGS=-g -I../../include
LDFLAGS=-L../../binary -lcassy -lusb-1.0 -Wl,-rpath=../../binary

clean:
	rm -f checkleds checkmemory relay squaresrc autoscanning
%:
	$(CC) $(CFLAGS) $@.c $(LDFLAGS) -o $@
