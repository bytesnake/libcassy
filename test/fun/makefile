CC=gcc
CFLAGS=-g -I../../include
LDFLAGS=-L../../binary -lcassy -Wl,-rpath=../../binary

all: oscillowav
oscillowav: oscillowav.c
	$(CC) $(CFLAGS) -DCA_DEVNODES oscillowav.c $(LDFLAGS) -o oscillowav
clean:
	rm -f oscillowav
