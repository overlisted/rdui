CC = gcc
CFLAGS = -I. -O1 -g -Wall
LDFLAGS = -lX11 -lm -lpthread

MINGW32?=i686-w64-mingw32-

all: demo.exe demo

demo.exe: examples/demo.c
	$(MINGW32)$(CC) -g -fno-ident -mwindows -m32 -DCNFGOGL $(CFLAGS) -o $@ $^  -lgdi32 -lws2_32 -s -D_WIN32_WINNT=0x0600 -DWIN32 -liphlpapi -lopengl32 -DMINGW_BUILD

demo: examples/demo.c
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

install:
	mv demo $(PREFIX)/bin/rdui_example

run: demo
	./demo

clean:
	rm demo demo.exe
