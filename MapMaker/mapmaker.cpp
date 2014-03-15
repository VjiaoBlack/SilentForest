#import "SDL/SDL.h"
#import "id.h"
#import <stdio.h>
#import <stdlib.h>
#import <map>
#import <list>

using namespace std;

#define tiles(x,y) tiles[y*width + x]

// Graphic class ################################
class Graphic {
public: 
    int height, width;
    int srcx, srcy;
    Graphic(int, int, int, int, int);
};

Graphic::Graphic(int h, int w, int sx, int sy, int id) {
    height = h;
    width = w;
    srcx = sx;
    srcy = sy; 
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
    std::list<Object> objects;

    Tile(int, int, int, Graphic*);
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
// code starts ##################################

int main(int argc, char* argv[]) {
    int which = -1; // -1 is uninitialized, 0 is tiles, 1 is items, 2 is special
    int end = 0;
    int x, y, id;
    char* pointer; // each pointer gets 10 chars.
    char test;

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
                    case -1: // currently does not have a specific scan into data.
                        fgets(cmd, 16, mapfile);
                        if (*cmd != ']')
                            printf("scanning for next command...\n");
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
                        }
                        break;
                    case 0: // currently scanning into tiles.
                        if (fscanf(mapfile,"{pos:%d;%d|id:%d}\n", &x, &y, &id)) {
                            printf("changed tile at %d, %d to id:%d\n", x, y, id);
                            change_tile(x,y,id);
                        } else {
                            which = -1;
                            printf("tile input syntax broken; tile input has ended.\n");
                        }
                        break;
                    case 1:
                        if (fscanf(mapfile,"%d;%d:{",&x,&y)) {
                            while(fscanf(mapfile,"%d",&id)) {
                                tiles(x,y).additem(id);
                                if ( (test = fgetc(mapfile)) == '}') { // normally would be commas
                                    break;
                                }
                            }
                        } else {
                            which = -1;
                            printf("item input syntax broken; item input has ended.\n");
                        }
                        break;
                    case 2:
                        // i have no idea what to do for this, gg.
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


    printf("--------------\n");
    printf("drawing...\n");
    init();

    while (running) {
        get_input();   

        update();

        //draws the scene

        SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 0, 0, 0));
        draw();
        // drawSprite(grass, 
        //         screen,
        //         0,0, 
        //         0,0,
        //         32,32);
        SDL_Flip(screen);

        /* Sleep briefly to stop sucking up all the CPU time */
        SDL_Delay(16);
    }
    SDL_FreeSurface(screen);
    /* Exit the program */
    SDL_Quit();
    exit(0);
    return 0;
}

void load_graphics() {
    // somehow put graphics data here! yay! wow! smart!!!
}

void init() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("Could not initialize SDL: %s\n", SDL_GetError());
        exit(1);
    }
    
    /* Open a 640 x 480 screen */
    screen = SDL_SetVideoMode( 640, 480, 0, SDL_HWSURFACE | SDL_DOUBLEBUF );
    grass = SDL_LoadBMP("grass.bmp");
    water = SDL_LoadBMP("water.bmp");

    // sprite.insert( std::pair<int,SDL_Surface*>(1,grass));
    // sprite.insert( std::pair<int,SDL_Surface*>(2,water));

    sprite[1] = grass;
    sprite[2] = water;

    if (screen == NULL) {
        printf("Couldn't set screen mode to 640 x 480: %s\n", SDL_GetError());
        exit(1);
    }
    
    /* Set the screen title */ // ???
    SDL_WM_SetCaption("Silent Forest Map Editor", "Editor");

    running = 1;
}

void get_input() {
    SDL_Event event;
    
    /* Loop through waiting messages and process them */
    if (SDL_PollEvent(&event)) {

        switch (event.type) {
            /* Closing the Window or pressing Escape will exit the program */
            case SDL_QUIT:
                SDL_Quit();
                exit(0);
                break;
            case SDL_KEYUP:
                keysHeld[event.key.keysym.sym] = false; 
                break;
            case SDL_KEYDOWN:
                keysHeld[event.key.keysym.sym] = true;
                break;
            break;
        }
    }
}

void update() {
    if ( keysHeld[SDLK_ESCAPE] ) {
        running = false;
    }
}

void change_tile(int x, int y, int id) {
    // assumes the tile is in the middle of grid
    // note that the tiles are aligned; the 'id' is at the center of the tile.

    tiles(x,y).id = id;

    update_tile(x,y, id); // TODO: udpate neighbors
    update_tile(x+1,y, id); 
    update_tile(x,y+1, id); 
    update_tile(x-1,y, id); 
    update_tile(x,y-1, id); 
}

void update_tile(int x, int y, int id) {
    // doesnt change id, just updates tile.
    int whichx = 0, whichy = 0; // which complete tile to take minitiles from
    int a, b, c; // is neighbor id equal to urs?

    // Graphic* graphics[4] = {
    //     new Graphic(16, 16, 0, 0, id), // top left
    //     new Graphic(16, 16, 16, 0, id), // top right
    //     new Graphic(16, 16, 0, 16, id), // bottom left
    //     new Graphic(16, 16, 16, 16, id) // bottom right
    // };
    // tiles(x,y).graphic = graphics;


    // note: make these switch cases into some kind of arrays.
    // Come on victor! You know how to shorten code.

    Graphic graphics[4] = {
        *new Graphic(16, 16, 0, 0, id),
        *new Graphic(16, 16, 1, 0, id),
        *new Graphic(16, 16, 0, 1, id),
        *new Graphic(16, 16, 1, 1, id)
    };



    // top left 
    a = tiles(x-1,y).id == id; // left
    b = tiles(x-1,y-1).id == id; // top left
    c = tiles(x,y-1).id == id; // top


    switch (a + b*2 + c*4) {
        case 0: // none
            whichx = 8;
            whichy = 0;
            break;
        case 1: // left
            whichx = 4;
            whichy = 0;
            break;
        case 2: // top left == none
            whichx = 8;
            whichy = 0;
            break;
        case 3: // top left, left == left
            whichx = 4;
            whichy = 0;
            break;
        case 4: // top 
            whichx = 6;
            whichy = 0;
            break; 
        case 5: // top, left
            whichx = 2;
            whichy = 0;
            break;
        case 6: // top, top left == top
            whichx = 6;
            whichy = 0;
            break;
        case 7: // top, top left, left
            whichx = 0;
            whichy = 0;
            break;
    }
    graphics[0] = *new Graphic(16, 16, 0/*whichx*16*/, 0/*whichy*16*/, id); // shoud just modify.. but w.e

    // top right
    a = tiles(x,y-1).id == id; // top
    b = tiles(x+1,y-1).id == id; // top right
    c = tiles(x+1,y).id == id; // right

    switch (a + b*2 + c*4) {
        case 0: // none
            whichx = 9;
            whichy = 0;
            break;
        case 1: // top
            whichx = 5;
            whichy = 0;
            break;
        case 2: // top right
            whichx = 9;
            whichy = 0;
            break;
        case 3: // top, top right
            whichx = 5;
            whichy = 0;
            break;
        case 4: // right
            whichx = 7;
            whichy = 0;
            break;
        case 5: // top, right
            whichx = 3;
            whichy = 0;
            break;
        case 6: // top right, right
            whichx = 7;
            whichy = 0;
            break;
        case 7: // right, top right, top
            whichx = 1;
            whichy = 0;
            break;
    }

    graphics[1] = *new Graphic(16, 16, 0/*whichx*16*/, 0/*whichy*16*/, id);




    // bottom left
    a = tiles(x,y+1).id == id; // bottom
    b = tiles(x-1,y+1).id == id; // bottom left
    c = tiles(x-1,y).id == id; // left

    switch (a + b*2 + c*4) {
        case 0: // none
            whichx = 8;
            whichy = 1;
            break;
        case 1: // bottom left
            whichx = 4;
            whichy = 1;
            break;
        case 2: // left
            whichx = 8;
            whichy = 1;
            break;
        case 3: // bottom left, bottom
            whichx = 4;
            whichy = 1;
            break;
        case 4: // left
            whichx = 6;
            whichy = 1;
            break;
        case 5: // bottom, left
            whichx = 2;
            whichy = 1;
            break;
        case 6: // bottom left, left
            whichx = 6;
            whichy = 1;
            break;
        case 7: // bottom, bottom left, left
            whichx = 0;
            whichy = 1;
            break;
    }

    graphics[2] = *new Graphic(16, 16, 0/*whichx*16*/, 0/*whichy*16*/, id);



    // bottom right
    a = tiles(x+1,y).id == id; // right
    b = tiles(x+1,y-1).id == id; // bottom right
    c = tiles(x,y-1).id == id; // bottom

    switch (a + b*2 + c*4) {
        case 0: // none
            whichx = 9;
            whichy = 1;
            break;
        case 1: // right
            whichx = 5;
            whichy = 1;
            break;
        case 2: // bottom right
            whichx = 9;
            whichy = 1;
            break;
        case 3: // right, bottom right
            whichx = 5;
            whichy = 1;
            break;
        case 4: // bottom
            whichx = 7;
            whichy = 1;
            break;
        case 5: // bottom, right
            whichx = 3;
            whichy = 1;
            break;
        case 6: // bottom right, bottom
            whichx = 7;
            whichy = 1;
            break;
        case 7: // right, bottom right, bottom
            whichx = 1;
            whichy = 1;
            break;
    }

    graphics[3] = *new Graphic(16, 16, 0/*whichx*16*/, 0/*whichy*16*/, id);

    tiles(x,y) = new Tile(id, x, y, graphics);


}

void draw() {
    int dx = 0, dy = 0;
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            dx = x * 32;
            dy = y * 32;
            // printf("%d, %d\n", x, y);


            printf("%d:", tiles(x,y).id);
            //drawSprite(sprite[tiles(x,y).id]/*tiles(x,y).graphic[0].image*/,screen,/*tiles(x,y).graphic[0].srcx*/0,/*tiles(x,y).graphic[0].srcy*/0,dx,dy,/*tiles(x,y).graphic[0].height*/32,32/*tiles(x,y).graphic[0].width*/);
            drawSprite(sprite[tiles(x,y).id], screen, tiles(x,y).graphic[0].srcx = 0, tiles(x,y).graphic[0].srcy = 0, dx   , dy   , 16, 16);
            printf("0>%d.%d,", tiles(x,y).graphic[0].srcx, tiles(x,y).graphic[0].srcy);// something wrong with this
            drawSprite(sprite[tiles(x,y).id], screen, tiles(x,y).graphic[1].srcx = 16, tiles(x,y).graphic[1].srcy = 0, dx+16, dy   , 16, 16);
            printf("1>%d.%d,", tiles(x,y).graphic[1].srcx, tiles(x,y).graphic[1].srcy); // and this
            drawSprite(sprite[tiles(x,y).id], screen, tiles(x,y).graphic[2].srcx, tiles(x,y).graphic[2].srcy, dx   , dy+16, 16, 16);
            printf("2>%d.%d,", tiles(x,y).graphic[2].srcx, tiles(x,y).graphic[2].srcy);
            drawSprite(sprite[tiles(x,y).id], screen, tiles(x,y).graphic[3].srcx, tiles(x,y).graphic[3].srcy, dx+16, dy+16, 16, 16);
            printf("3>%d.%d   \n", tiles(x,y).graphic[3].srcx, tiles(x,y).graphic[3].srcy);
        }
        printf("\n");
    }
}

Tile* parse_tile(char* tiledata) {
    return NULL;
}

void init_map(int id) {
    int x, y;
    tiles = new Tile[height * width];
    printf("tiles should be initilaized\n");
    for (y = 0; y < height; y++) {
        for (x = 0; x < width; x++) {
            if (x > 0 && x < width - 1 && y > 0 && y < height - 1) {
                update_tile(x,y,id);
            }

            Graphic graphics[4] = {
                *new Graphic(16, 16, 0, 0, id),
                *new Graphic(16, 16, 0, 0, id),
                *new Graphic(16, 16, 0, 0, id),
                *new Graphic(16, 16, 0, 0, id)
            };
            tiles(x,y) = new Tile(id, x, y, graphics); // weird large number shud be id
        }
    }
}

char* data_to_string() {
    printf("error! data_to_string has not yet been implemented.\n");
    return "help";
}

void drawSprite(SDL_Surface* imageSurface, 
                SDL_Surface* screenSurface,
                int sx, int sy, 
                int dx, int dy,
                int w, int h) {
   SDL_Rect srcRect;
   srcRect.x = sx;
   srcRect.y = sy;
   srcRect.w = w;
   srcRect.h = h;

   SDL_Rect dstRect;
   dstRect.x = dx;
   dstRect.y = dy;
   dstRect.w = w;
   dstRect.h = h;

   SDL_BlitSurface(imageSurface, &srcRect, screenSurface, &dstRect);
}

