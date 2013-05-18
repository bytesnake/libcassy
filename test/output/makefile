CC=gcc
CFLAGS=-g -I../../include -DCA_DEVNODES
LDFLAGS=-L../../binary -lcassy -Wl,-rpath=../../binary

clean:
	rm -f analogout generator
%:
	$(CC) $(CFLAGS) $@.c $(LDFLAGS) -o $@
