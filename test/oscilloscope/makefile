CC=gcc
CFLAGS=-g -I../../include
LDFLAGS=-L../../binary -lcassy -Wl,-rpath=../../binary

all: oscillocont oscillotest readstream
oscillocont: oscillocont.c
	$(CC) $(CFLAGS) -DCA_DEVNODES oscillocont.c $(LDFLAGS) -o oscillocont
oscillotest: oscillotest.c
	$(CC) $(CFLAGS) -DCA_DEVNODES oscillotest.c $(LDFLAGS) -o oscillotest
readstream: readstream.c
	$(CC) $(CFLAGS) -DCA_DEVNODES readstream.c $(LDFLAGS) -o readstream
clean:
	rm -f oscillocont oscillotest readstream
