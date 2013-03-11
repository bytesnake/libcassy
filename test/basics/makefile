CC=gcc
CFLAGS=-g -I../../include
LDFLAGS=-L../../binary -lcassy -Wl,-rpath=../../binary

clean:
	rm -f checkleds checkmemory relay
%: $@.c
	$(CC) $(CFLAGS) $@.c $(LDFLAGS) -o $@