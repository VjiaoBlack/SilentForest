all: mapmaker

mapmaker.o: mapmaker.cpp mapmaker.h 
	g++ -c mapmaker.cpp -o mapmaker.o `sdl-config --cflags --libs` -Wall -Wextra

font.o: font.cpp font.h
	g++ -c font.cpp -o font.o -Wall -Wextra

mapmaker: mapmaker.o font.o
	g++ mapmaker.o font.o -o mapmaker `sdl-config --cflags --libs` -Wall -Wextra

clean:
	rm mapmaker
	rm *.o
