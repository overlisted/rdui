rdui: rdui.c rawdraw/rawdraw.c
	gcc -o $@ $^ -lX11 -lm -lpthread -lXinerama -lXext -lGL -g
