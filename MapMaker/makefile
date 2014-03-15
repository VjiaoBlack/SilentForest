all: mapmaker

mapmaker: mapmaker.cpp mapmaker.h
	g++ mapmaker.cpp -o mapmaker `sdl-config --cflags --libs` -Wall -Wextra

clean:
	rm mapmaker
