CC=cc
CFLAGS=-shared -Wall -g -fPIC
LDFLAGS=-ldl -lm
SOURCES=ca_misc.c ca_error.c ca_data.c ca_stream.c ca_inout.c functions/ca_utility.c functions/ca_analog.c functions/ca_relay.c functions/ca_digital.c functions/ca_oscilloscope.c
OUTPUT=../binary/libcassy.so

all:
	$(CC) $(CFLAGS) $(SOURCES) $(LDFLAGS) -o $(OUTPUT)
clear:
	rm -f $(OUTPUT)
