#include "SDL/SDL.h"
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


// class Object {
//     /*
//     int id; // 0-999 equippable, 1000-1999 usable, 2000-2999 affects gameplay (keys, talismen, extra backpack, etc) ...
//     */
// };

// class Stat {
//     /*
//     int value;
//     */
// };

// class Relationship {
//     /*
//     // Human* object; // how do i put the decl of human before this??
//     int parts[6]; // Trust, Attraction, Good-Will, Respect, Influence (that u have over them), Contentment (opposite of anger).
//     */
// };

// class Human {
//     /*
//     int hp, mp;
//     Stat stats[6]; // str, dex, con, int, wis, char
//     // implement personality
//     Relationship relation; // relationship with character; this should really be an ordered arraylist.
//     */
// };

#define tiles(x,y) tiles.at((y)*width + (x))
#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 800


int gridx;
int gridy;
int sx;
int sy;

int delay;  


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

SDL_Surface* bitmap;
SDL_Surface* tree;
SDL_Surface* board;
SDL_Surface* intro;

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
void get_input();
void menu_loop();
void draw_character();
void load_graphics();

