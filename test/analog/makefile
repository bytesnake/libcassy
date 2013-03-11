CC=gcc
CFLAGS=-g -I../../include
LDFLAGS=-L../../binary -lcassy -Wl,-rpath=../../binary

%: $@.c
	$(CC) $(CFLAGS) $@.c $(LDFLAGS) -o $@
