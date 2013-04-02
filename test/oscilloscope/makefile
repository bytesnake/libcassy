CC=gcc
CFLAGS=-x c -g -I../../include
LDFLAGS=-L../../binary -lcassy -lusb-1.0 -Wl,-rpath=../../binary

clean:
	rm -f oscillotest oscillocont readstream
%:
	$(CC) $(CFLAGS) $@.c $(LDFLAGS) -o $@
