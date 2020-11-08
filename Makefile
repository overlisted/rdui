example: example.c rdui.c rawdraw/CNFG.c
	gcc -o $@ $^ -lX11 -lm -lpthread -lXinerama -lXext -lGL -g

install:
	mv example $(PREFIX)/bin

run:
	./example

rdui: rdui.c rawdraw/CNFG.c
	gcc -o $@ $^ -lX11 -lm -lpthread -lXinerama -lXext -lGL -g
