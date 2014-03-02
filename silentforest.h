#include "SDL/SDL.h"
#include <stdio.h>

class Tile {
public:
    int id; // 0-999 unwalkable, 1000-1999 walkable, 2000-2999 special tile (launches scene), 3000-3999 special tile (interactive) ...
    int gridx, gridy;
    int height, width;
    SDL_Surface* graphic;
    Tile(int, int, int, SDL_Surface*, int, int);
    Tile();
    // note that an actual grid should contain a linked list of tile at every cor.
};

Tile::Tile(int a, int b, int c, SDL_Surface* d, int h, int w) {
    id = a;
    gridx = b;
    gridy = c;
    graphic = d;
    height = h;
    width = w;
}

Tile::Tile() {
    id = 0;
    gridx = 0;
    gridy = 0;
    graphic = NULL;
    height = 0;
    width = 0;
}

class Object {
    int id; // 0-999 equippable, 1000-1999 usable, 2000-2999 affects gameplay (keys, talismen, extra backpack, etc) ...
};

class Stat {
    int value;
};

class Relationship {
    // Human* object; // how do i put the decl of human before this??
    int parts[6]; // Trust, Attraction, Good-Will, Respect, Influence (that u have over them), Contentment (opposite of anger).
};

class Human {
    int hp, mp;
    Stat stats[6]; // str, dex, con, int, wis, char
    // implement personality
    Relationship relation; // relationship with character; this should really be an ordered arraylist.
};

void getInput();
void cleanup();
void init(char *title);
void update();
void draw();

Tile grid[15][20];

int SCREEN_WIDTH = 640;
int SCREEN_HEIGHT = 480;
int running;

int xoffset, yoffset, sx, sy, gridx, gridy;
int delay;

bool keysHeld[323] = {false};

SDL_Surface *screen;
SDL_Surface *grass;
SDL_Surface *bitmap;
SDL_Surface *tree;

void drawSprite(SDL_Surface*, SDL_Surface*, int, int, int, int, int, int);
