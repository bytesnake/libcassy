CC=gcc
CFLAGS=-x c -g -I../../include -DCA_DEVNODES
LDFLAGS=-L../../binary -lcassy -Wl,-rpath=../../binary

clean:
	rm -f oscillowav
%:
	$(CC) $(CFLAGS) $@.c $(LDFLAGS) -o $@
