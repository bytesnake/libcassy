CC=gcc
CFLAGS=-g -I../../include
LDFLAGS=-L../../binary -lcassy -Wl,-rpath=../../binary

all: plotdata simpleinput
plotdata: plotdata.c
	$(CC) $(CFLAGS) -DCA_DEVNODES plotdata.c $(LDFLAGS) -o plotdata
simpleinput: simpleinput.c
	$(CC) $(CFLAGS) -DCA_DEVNODES simpleinput.c $(LDFLAGS) -o simpleinput
clean:
	rm -f plotdata simpleinput
