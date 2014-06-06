CC=gcc
CFLAGS=-Wall -Werror -Wextra -std=c99 -pedantic -Wno-unused-parameter
# D_BSD_SOURCE for strsep
LIBS=-lncurses -lm -D_BSD_SOURCE

all: stag
stag:
	$(CC) $(CFLAGS) $(LIBS) stag.c view.c data.c -o stag
clean:
	rm -rf *.o stag
