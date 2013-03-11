CC=cc
CFLAGS=-shared -Wall -fPIC
LDFLAGS=-ldl -lm
SOURCES=ca_misc.c ca_error.c ca_data.c ca_stream.c ca_inout.c functions/ca_utility.c functions/ca_analog.c functions/ca_relay.c functions/ca_digital.c functions/ca_oscilloscope.c
OUTPUT=../binary/libcassy.so

all:
	$(CC) -g $(CFLAGS) $(SOURCES) $(LDFLAGS) -o $(OUTPUT)
release:
	$(CC) -O3 $(CFLAGS) $(SOURCES) $(LDFLAGS) -o $(OUTPUT)
debug:
	$(CC) -g -D CA_DEBUG_PRINT $(CFLAGS) $(SOURCES) $(LDFLAGS) -o $(OUTPUT)
nowrite:
	$(CC) -g -D CA_DEBUG_NOWRITE $(CFLAGS) $(SOURCES) $(LDFLAGS) -o $(OUTPUT)
noread:
	$(CC) -g -D CA_DEBUG_NOREAD $(CFLAGS) $(SOURCES) $(LDFLAGS) -o $(OUTPUT)
clear:
	rm -f $(OUTPUT)
