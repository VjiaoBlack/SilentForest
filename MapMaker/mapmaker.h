#include "SDL/SDL.h"
#include "id.h"

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



static SDL_Surface* font;
static SDL_Surface* screen;


void drawText(char*, int, int);
int drawLetter(char, int, int);
void drawSprite(SDL_Surface*, SDL_Surface*, int, int, int, int, int, int);





// Graphic class ################################
class Graphic {
public: 
    int height, width;
    int srcx, srcy;

    Graphic& operator= (const Graphic *g) {
        height = g->height;
        width = g->width;
        srcx = g->srcx;
        srcy = g->srcy;
        return *this;
    }
    Graphic(int, int, int, int, int);
    Graphic();
};

Graphic::Graphic(int h, int w, int sx, int sy, int id) {
    height = h;
    width = w;
    srcx = sx;
    srcy = sy; 
    height += 0 * id; // suppresses warnings
}

Graphic::Graphic() {
    height = 16;
    width = 16;
    srcx = 0;
    srcy = 0;
}


// Object class #################################
class Object {
public:
    int id;
    Graphic* graphic;
    Object(int, Graphic*);
    Object(int);
    Object();
};

Object::Object(int identifier, Graphic* graphicpointer) {
    id = identifier;
    graphic = graphicpointer;
}

Object::Object(int identifier) {
    // find the correct graphic...
    id = identifier;
    graphic = NULL;
}

Object::Object() {
    id = 0;
    graphic = NULL;
}

// Tile class ###################################
class Tile {
public:
    int id; // 0-999 walkable, 1000-1999 unwalkable, 2000-2999 special tile (launches scene), 3000-3999 special tile (interactive) ...
    int gridx, gridy;
    std::vector<Graphic> graphic;
    std::list<Object> objects;
    std::map<std::string,std::string> specials;

    Tile(int, int, int, std::vector<Graphic>);
    Tile(int,int,int);
    Tile();
    Tile& operator= (const Tile *t) {
        id = t->id;
        gridx = t->gridx;
        gridy = t->gridy;
        graphic = t->graphic;
        return *this;
    }

    void additem(int id) {
        printf("(placeholder) item of id:%d added to tile at %d,%d.\n", id, gridx, gridy);
        Object* object = new Object(id);
        objects.push_back(*object);
        return;
    }
    // note that an actual grid should contain a linked list of tile at every cor.
};

Tile::Tile(int identifier, int gridxcor, int gridycor, std::vector<Graphic> graphicpointer) {
    id = identifier;
    gridx = gridxcor;
    gridy = gridycor;
    graphic = graphicpointer;
    objects.resize(0);
}

Tile::Tile(int ident, int grdx, int grdy) {
    id = ident;
    gridx = grdx;
    gridy = grdy;
    graphic.resize(4);
    objects.resize(0);
}

Tile::Tile() {
    id = 0;
    gridx = 0;
    gridy = 0;
    graphic.resize(4);
    objects.resize(0);
}

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


void init_map(int);
void update();
void init();
void draw();
void get_input();
void change_tile(int,int,int);
void update_tile(int,int,int);
std::string parse_tile(Tile*);
std::string data_to_string();
void edit_objects(int,int);
void edit_specials(int,int);
void put_special(int, int, std::string, std::string);

