CC = gcc
CFLAGS = -O1 -g
LDFLAGS = -lX11 -lm -lpthread

FILES = rdui.c rawdraw/CNFG.c default-elements.c util.c

example: $(FILES) example.c
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

install:
	mv example $(PREFIX)/bin/rdui_example

run: example
	./example

rdui: $(FILES)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

clean:
	rm example
