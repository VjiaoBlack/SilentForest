// #import "SDL/SDL.h"
#import "id.h"
#import <stdio.h>
#import <stdlib.h>
#define tiles(x,y) tiles[y*width + x]

// Graphic class ################################
class Graphic {
public: 
    int height, width;
    int srcx, srcy;
/*    SDL_Surface* image;*/
    Graphic(int, int, int, int/*, SDL_Surface**/);
};

Graphic::Graphic(int h, int w, int sx, int sy/*, SDL_Surface* img*/) {
    height = h;
    width = w;
    srcx = sx;
    srcy = sy; 
    // image = img;
}

// Object class #################################
class Object {
public:
    int id;
    Graphic* graphic;
    Object(int, Graphic*);
};

Object::Object(int identifier, Graphic* graphicpointer) {
    id = identifier;
    graphic = graphicpointer;
}

// Tile class ###################################
class Tile {
public:
    int id; // 0-999 walkable, 1000-1999 unwalkable, 2000-2999 special tile (launches scene), 3000-3999 special tile (interactive) ...
    int gridx, gridy;
    Graphic* graphic;
    Tile(int, int, int, Graphic*);
    Tile();
    Tile& operator= (const Tile *t) {
        id = t->id;
        gridx = t->gridx;
        gridy = t->gridy;
        graphic = t->graphic;
        return *this;
    }
    // note that an actual grid should contain a linked list of tile at every cor.
};

Tile::Tile(int identifier, int gridxcor, int gridycor, Graphic* graphicpointer) {
    id = identifier;
    gridx = gridxcor;
    gridy = gridycor;
    graphic = graphicpointer;
}

Tile::Tile() {
    id = 0;
    gridx = 0;
    gridy = 0;
    graphic = NULL;
}



Tile* tiles;
int width;
int height;
int base_tile_id;

// SDL_Surface* screen;

void init_map(int);

// code starts ##################################

int main(int argc, char* argv[]) {
    int which = -1; // -1 is uninitialized, 0 is tiles, 1 is items, 2 is special
    int end = 0;

    char cmd[32];
    if (argc == 2) {
        FILE* mapfile;
        if ( (mapfile = fopen(argv[1], "r")) ) {
            end = feof(mapfile);
            fscanf(mapfile, "h:%d w:%d b:{id:%d}\n", &height, &width, &base_tile_id);
            printf("scanned: height is %d, width is %d, base tile id is %d\n", height, width, base_tile_id);
            init_map(base_tile_id);
            printf("initialized map with given constraints.\n");
            while (!end) {
                switch (which) {
                    case -1: // currently does not have a 
                        printf("scanning for next command...\n");
                        fgets(cmd, 16, mapfile);
                        switch (*cmd) {
                            case 't':
                                which = 0;
                                printf("next command is |tiles|.\n");
                                break;
                            case 'i':
                                which = 1;
                                printf("next command is |items|.\n");
                                break;
                            case 's':
                                which = 2;
                                printf("next command is |special|.\n");
                                break;
                            case 'e':
                                end = 1;
                                printf("next command is |end|.\n");
                                break;
                            case ']':
                                printf("|%s| has ended.\n", cmd);
                                which = -1;
                                break;
                        }
                        break;
                    case 0:
                        break;
                    case 1:
                        break;
                    case 2:
                        break;
                }

            }
        } else {
            mapfile = fopen(argv[1], "w");
            width = 20;
            height = 15;
        }

    } else {
        printf("./mapmaker <mapname>\n");
    }

    printf("ending...\n");
    return 0;
}

// void init() {
//     if (SDL_Init(SDL_INIT_VIDEO) < 0) {
//         printf("Could not initialize SDL: %s\n", SDL_GetError());
//         exit(1);
//     }
    
//     /* Open a 640 x 480 screen */
//     screen = SDL_SetVideoMode( 640, 480, 0, SDL_HWSURFACE | SDL_DOUBLEBUF );
//     if (screen == NULL) {
//         printf("Couldn't set screen mode to 640 x 480: %s\n", SDL_GetError());
//         exit(1);
//     }
    
//     /* Set the screen title */ // ???
//     SDL_WM_SetCaption("Silent Forest Map Editor", "Editor");
// }

// Tile* parse_tile(char* tiledata) {
//     return NULL;
// }

void init_map(int id) {
    int x, y;
    tiles = new Tile[height * width];
    printf("tiles is apparently initilaized\n");
    for (y = 0; y < height; y++) {
        for (x = 0; x < width; x++) {
            tiles(x,y) = new Tile(id, x, y, NULL);
        }
    }
}
