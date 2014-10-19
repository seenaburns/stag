CC ?= gcc
CFLAGS=-Wall -Werror -Wextra -std=c99 -pedantic -Wno-unused-parameter
# D_BSD_SOURCE for strsep
LIBS=-lncurses -lm -D_DEFAULT_SOURCE
PREFIX ?= /usr/local

.PHONY: install uninstall clean

all: stag

stag:
	$(CC) $(CFLAGS) stag.c view.c data.c -o stag $(LIBS) 

install:
	install -d "$(DESTDIR)$(PREFIX)/bin"
	install -d "$(DESTDIR)$(PREFIX)/share/man/man1"
	install -m755 stag "$(DESTDIR)$(PREFIX)/bin/stag"
	install -m644 doc/stag.1 "$(DESTDIR)$(PREFIX)/share/man/man1/stag.1"

uninstall:
	rm "$(DESTDIR)$(PREFIX)/bin/stag"
	rm "$(DESTDIR)$(PREFIX)/share/man/man1/stag.1"

clean:
	rm -rf *.o stag
