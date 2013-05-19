CC=gcc
CFLAGS=-g -I../../include
LDFLAGS=-L../../binary -lcassy -Wl,-rpath=../../binary

all: analogout generator
analogout: analogout.c
	$(CC) $(CFLAGS) analogout.c $(LDFLAGS) -o analogout
generator: generator.c
	$(CC) $(CFLAGS) generator.c $(LDFLAGS) -o generator
clean:
	rm -f analogout generator
