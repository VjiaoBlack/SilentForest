#include <stdio.h>
#include <stdlib.h>
#include <map>
#include "SDL/SDL.h"

typedef struct letter_t {
    int width;
    int xpos;
    int ypos;
} letter_t;

extern std::map<char,letter_t> letter_data;
extern SDL_Surface* font;
extern SDL_Surface* screen;

void init_font();
void drawText(char*, int, int);
int drawLetter(char, int, int);
void drawSprite(SDL_Surface*, SDL_Surface*, int, int, int, int, int, int);
