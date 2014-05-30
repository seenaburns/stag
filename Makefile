CC=gcc
CFLAGS=-Wall -Werror -Wextra -std=c99 -pedantic -Wno-unused-parameter
LIBS= -L/usr/local/opt/ncurses/lib -I/usr/local/opt/ncurses/include -lncurses

all: stag
stag:
	$(CC) $(CFLAGS) $(LIBS) stag.c view.c data.c -o stag
clean:
	rm -rf *.o stag
