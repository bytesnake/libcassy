CC=gcc
CFLAGS=-g -I../../include
LDFLAGS=-L../../binary -lcassy -Wl,-rpath=../../binary

all: oscillowav
oscillowav: oscillowav.c
	$(CC) $(CFLAGS) oscillowav.c $(LDFLAGS) -o oscillowav
clean:
	rm -f oscillowav
