all: silentforest

silentforest: silentforest.o
	g++ silentforest.o -o silentforest `sdl-config --cflags --libs` -Wall -Wextra

silentforest.o: silentforest.cpp
	g++ -c silentforest.cpp -o silentforest.o `sdl-config --cflags --libs` -Wall -Wextra

clean:
	rm silentforest
	rm *.o
