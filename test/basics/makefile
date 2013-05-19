CC=gcc
CFLAGS=-g -I../../include
LDFLAGS=-L../../binary -lcassy -Wl,-rpath=../../binary

all: autoscanning checkleds checkmemory relay squaresrc
autoscanning: autoscanning.c
	$(CC) $(CFLAGS) autoscanning.c $(LDFLAGS) -lusb-1.0 -o autoscanning
checkleds: checkleds.c
	$(CC) $(CFLAGS) checkleds.c $(LDFLAGS) -o checkleds
checkmemory: checkmemory.c
	$(CC) $(CFLAGS) checkmemory.c $(LDFLAGS) -o checkmemory
relay: relay.c
	$(CC) $(CFLAGS) relay.c $(LDFLAGS) -o relay
squaresrc: squaresrc.c
	$(CC) $(CFLAGS) squaresrc.c $(LDFLAGS) -o squaresrc
clean:
	rm -f autoscanning checkleds checkmemory relay squaresrc
