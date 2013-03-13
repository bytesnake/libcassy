CC=gcc
CFLAGS=-g -I../../include
LDFLAGS=-L../../binary -lcassy -Wl,-rpath=../../binary

clean:
	rm -f plotdata simpleinput
%:
	$(CC) $(CFLAGS) $@.c $(LDFLAGS) -o $@
