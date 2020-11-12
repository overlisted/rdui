CC = gcc
CFLAGS = -O1 -g
LDFLAGS = -lX11 -lm -lpthread

example: example.c
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

install:
	mv example $(PREFIX)/bin/rdui_example

run: example
	./example

clean:
	rm example
