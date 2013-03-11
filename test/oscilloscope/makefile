CC=gcc
CFLAGS=-g -I../../include
LDFLAGS=-L../../binary -lcassy -Wl,-rpath=../../binary

clean:
	rm -f oscillotest readstream
%: $@.c
	$(CC) $(CFLAGS) $@.c $(LDFLAGS) -o $@