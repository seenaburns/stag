CC=gcc
CFLAGS=-Wall -Werror -Wextra -std=c99 -pedantic -Wno-unused-parameter

all: stag
stag:
	$(CC) $(CFLAGS) stag.c -o stag
clean:
	rm -rf *.o stag
