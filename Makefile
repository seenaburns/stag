CC ?= gcc
CFLAGS=-Wall -Werror -Wextra -std=c99 -pedantic -Wno-unused-parameter
# D_BSD_SOURCE for strsep
LIBS=-lncurses -lm -D_BSD_SOURCE
PREFIX ?= /usr/local/

all: stag
stag:
	$(CC) $(CFLAGS) stag.c view.c data.c -o stag $(LIBS) 
install:
	cp stag "$(PREFIX)bin"
	mkdir -p "$(PREFIX)share/man/man1/"
	cp doc/stag.1 "$(PREFIX)share/man/man1/"
uninstall:
	rm "$(PREFIX)bin/stag"
	rm "$(PREFIX)share/man/man1/stag.1"
clean:
	rm -rf *.o stag
