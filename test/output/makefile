CC=gcc
CFLAGS=-g -I../../include
LDFLAGS=-L../../binary -lcassy -Wl,-rpath=../../binary

all: analogout generator
analogout: analogout.c
	$(CC) $(CFLAGS) -DCA_DEVNODES analogout.c $(LDFLAGS) -o analogout
generator: generator.c
	$(CC) $(CFLAGS) -DCA_DEVNODES generator.c $(LDFLAGS) -o generator
clean:
	rm -f analogout generator
