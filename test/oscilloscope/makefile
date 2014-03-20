CC=gcc
CFLAGS=-g -I../../include
LDFLAGS=-L../../binary -lcassy -Wl,-rpath=../../binary

all: oscillocont oscillotest readstream
oscillopulse: oscillopulse.c
	$(CC) $(CFLAGS) oscillopulse.c $(LDFLAGS) -o oscillopulse
oscillocont: oscillocont.c
	$(CC) $(CFLAGS) oscillocont.c $(LDFLAGS) -o oscillocont
oscillotest: oscillotest.c
	$(CC) $(CFLAGS) oscillotest.c $(LDFLAGS) -o oscillotest
readstream: readstream.c
	$(CC) $(CFLAGS) readstream.c $(LDFLAGS) -o readstream
benchmark: benchmark.c
	$(CC) $(CFLAGS) benchmark.c $(LDFLAGS) -o benchmark
clean:
	rm -f oscillocont oscillotest readstream
