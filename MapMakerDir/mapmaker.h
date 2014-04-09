#include "SDL/SDL.h"
#include "id.h"
#include "game.h"


#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <map>
#include <list>
#include <vector>
#include <iostream>
#include <fstream>
#include <string>


using namespace std;

#define tiles(x,y) tiles.at((y)*width + (x))



std::map<char,letter_t> letter_data;

SDL_Surface* font;
SDL_Surface* screen;

std::vector<Tile> tiles;

int width;
int height;
int base_tile_id;
int draw_id;
int selectedx;
int selectedy;

SDL_Surface* grass;
SDL_Surface* water; // these should be in an array, with id as the identifier.

std::map<int,SDL_Surface*> sprite;

int running;

bool keysHeld[323] = {false};
int mousex;
int mousey;
int which_disp; // 0 is items, 1 is specials
bool mouseleftdown;

int xoffset;
int yoffset;



void update();
void init();
void draw();
void draw_map();
void draw_menu();
void get_input();


