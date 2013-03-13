CC=gcc
CFLAGS=-x c -g -I../../include
LDFLAGS=-L../../binary -lcassy -Wl,-rpath=../../binary

clean:
	rm -f oscillotest readstream
%:
	$(CC) $(CFLAGS) $@.c $(LDFLAGS) -o $@
