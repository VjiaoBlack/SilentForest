all: silentforest

silentforest: silentforest.o font.o mapio.o game.o
	g++ silentforest.o font.o mapio.o game.o -o silentforest `sdl-config --cflags --libs` -Wall -Wextra

silentforest.o: silentforest.cpp silentforest.h game.h
	g++ -c silentforest.cpp -o silentforest.o `sdl-config --cflags --libs` -Wall -Wextra

font.o: font.cpp font.h 
	g++ -c font.cpp -o font.o -Wall -Wextra

mapio.o: mapio.cpp mapio.h game.h
	g++ -c mapio.cpp -o mapio.o -Wall -Wextra

game.o: game.cpp game.h
	g++ -c game.cpp -o game.o -Wall -Wextra

clean:
	rm silentforest
	rm *.o
