CC ?= gcc
CFLAGS=-Wall -Werror -Wextra -std=c99 -pedantic -Wno-unused-parameter
# D_BSD_SOURCE for strsep
LIBS=-lncurses -lm -D_BSD_SOURCE
PREFIX ?= /usr/local/

all: stag
stag:
	$(CC) $(CFLAGS) $(LIBS) stag.c view.c data.c -o stag
install:
	cp stag "$(PREFIX)/bin"
	cp doc/stag.1 "$(PREFIX)/share/man/man1"
uninstall:
	rm "$(PREFIX)/bin/stag"
	rm "$(PREFIX)/share/man/man1/stag.1"
clean:
	rm -rf *.o stag
