CC=gcc
CFLAGS=-shared -x c -Wall -O3 -fPIC
LDFLAGS=-ldl -lm
SOURCES=ca_data.c ca_dev.c ca_functions.c ca_main.c
OUTPUT=../binary/libcassy.so

all:
	$(CC) $(CFLAGS) $(SOURCES) $(LDFLAGS) -o $(OUTPUT)
clear:
	rm -f $(OUTPUT)
