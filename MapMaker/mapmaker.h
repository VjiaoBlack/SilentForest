#import "SDL/SDL.h"
#import "id.h"
#import "mapmaker.h"

#import <stdio.h>
#import <stdlib.h>
#import <map>
#import <list>
#import <array>
#import <iostream>


using namespace std;

#define tiles(x,y) tiles[y*width + x]

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
    srcx = 0;
    srcy = 0; 
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









// Tile class ###################################
class Tile {
public:
    int id; // 0-999 walkable, 1000-1999 unwalkable, 2000-2999 special tile (launches scene), 3000-3999 special tile (interactive) ...
    int gridx, gridy;
    Graphic* graphic;
    //std::array<Graphic> graphicpointer;
    std::list<Object> objects;

    Tile(int, int, int, Graphic*);
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

Tile::Tile(int identifier, int gridxcor, int gridycor, Graphic* graphicpointer) {
    id = identifier;
    gridx = gridxcor;
    gridy = gridycor;
    graphic = graphicpointer;
}

Tile::Tile(int ident, int grdx, int grdy) {
    id = ident;
    gridx = grdx;
    gridy = grdy;
    Graphic graphics[5];
    graphic = graphics;
}

Tile::Tile() {
    id = 0;
    gridx = 0;
    gridy = 0;
    Graphic graphics[5];
    graphic = graphics;
}

Tile** tiles;
int width;
int height;
int base_tile_id;

SDL_Surface* screen;
SDL_Surface* grass;
SDL_Surface* water; // these should be in an array, with id as the identifier.

std::map<int,SDL_Surface*> sprite;

int running;

bool keysHeld[323] = {false};

void init_map(int);
void update();
void init();
void draw();
void get_input();
void change_tile(int,int,int);
void update_tile(int,int,int);
void drawSprite(SDL_Surface*, SDL_Surface*, int, int, int, int, int, int);
