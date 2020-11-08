example: example.c rdui.c rawdraw/CNFG.c
	gcc -o $@ $^ -lX11 -lm -lpthread -lXinerama -lXext -lGL -g

rdui: rdui.c rawdraw/CNFG.c
	gcc -o $@ $^ -lX11 -lm -lpthread -lXinerama -lXext -lGL -g
