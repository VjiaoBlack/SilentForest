#include "SDL/SDL.h"
#include <stdio.h>

void getInput();
void cleanup();
void init(char *title);
void update();

int SCREEN_WIDTH = 640;
int SCREEN_HEIGHT = 480;
int running;

int x, y, sx, sy;
int delay;

bool keysHeld[323] = {false};

SDL_Surface *screen;
SDL_Surface *grass;
SDL_Surface *bitmap;
SDL_Surface *tree;

void drawSprite(SDL_Surface* imageSurface, 
                SDL_Surface* screenSurface,
                int sx, int sy, 
                int dx, int dy,
                int w, int h);
