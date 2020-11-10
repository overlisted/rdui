CC = gcc
CFLAGS = -O1 -g
LDFLAGS = -lX11 -lm -lpthread

example: example.c rdui.c rawdraw/CNFG.c default-elements.c util.c
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

install:
	mv example $(PREFIX)/bin/rdui_example

run: example
	./example

rdui: rdui.c rawdraw/CNFG.c default-elements.c util.c
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

clean:
	rm example
