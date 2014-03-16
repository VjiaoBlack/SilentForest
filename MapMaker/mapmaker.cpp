
#include "mapmaker.h"

// code starts ##################################


int main(int argc, char* argv[]) {
    int which = -1; // -1 is uninitialized, 0 is tiles, 1 is items, 2 is special
    int end = 0;
    int x, y, id;
    char* pointer; // each pointer gets 10 chars. // what does this do again
    char test;

    char cmd[32];
    if (argc == 2) {
        FILE* mapfile;
        if ( (mapfile = fopen(argv[1], "r")) ) {
            end = feof(mapfile);
            height = 10;
            width = 15;
            fscanf(mapfile, "h:%d w:%d b:{id:%d}\n", &height, &width, &base_tile_id);
            printf("scanned: height is %d, width is %d, base tile id is %d\n", height, width, base_tile_id);
            

            init_map(base_tile_id = 1);

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
                                //tiles(x,y).additem(id);
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
        return -1;
    }

    printf("ending...\n");

    printf("--------------\n");
    printf("drawing...\n");

    init();
    for (y = 0; y < height; y++) {
        for (x = 0; x < width; x++) {
            update_tile(x,y,tiles(x,y).id);
        }
    }
    SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 0, 0, 0));
        draw();
        SDL_Flip(screen);

    while (running) {
        get_input();  

        update();

        //draws the scene

        // SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 0, 0, 0));
        // draw();
        // SDL_Flip(screen);

         // Sleep briefly to stop sucking up all the CPU time 
       SDL_Delay(16);
    }
    SDL_FreeSurface(screen);
     // Exit the program 
    SDL_Quit();
    exit(0);
    return 0;
}

void load_graphics() {
    // somehow put input graphics data here! yay! wow! smart!!!
    // note how most of this is currently in main().

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
    // note that the tiles are aligned; the 'id' is at the center of the tile.

    tiles(x,y).id = id;

    update_tile(x,y, id); // TODO: udpate neighbors
    if (x<width) {
        if (y<height) update_tile(x+1,y+1,tiles(x+1,y+1).id);
        if (y>0) update_tile(x+1,y-1,tiles(x+1,y-1).id);    
        update_tile(x+1,y  ,tiles(x+1,y  ).id);
    } 
    if (x>0) {
        if (y<height) update_tile(x-1,y+1,tiles(x-1,y+1).id);
        if (y>0) update_tile(x-1,y-1,tiles(x-1,y-1).id);
        update_tile(x-1,y  ,tiles(x-1,y  ).id);
    } 
    if (y<height)update_tile(x  ,y+1,tiles(x  ,y+1).id);
    if (y>0) update_tile(x  ,y-1,tiles(x  ,y-1).id);
    update_tile(x  ,y  ,tiles(x  ,y  ).id);

}

void update_tile(int x, int y, int id) {
    // doesnt change id, just updates tile.
    int whichx = 0, whichy = 0; // which complete tile to take minitiles from
    int a, b, c; // is neighbor id equal to urs?
    if (id == 1) {
        return;
    }

    // note: this function needs to be shortened
    Graphic insert;

    // following a temp fix against seg-faulting for neighbors. Fix later.
    if (x > 0 && y > 0) {// top left 
        a = tiles(x-1,y).id == id; // left
        b = tiles(x-1,y-1).id == id; // top left
        c = tiles(x,y-1).id == id; // top
        switch (a + b*2 + c*4) {
            case 0: whichx = 8; whichy = 0; break;
            case 1: whichx = 4; whichy = 0; break;
            case 2: whichx = 8; whichy = 0; break;
            case 3: whichx = 4; whichy = 0; break;
            case 4: whichx = 6; whichy = 0; break; 
            case 5: whichx = 2; whichy = 0; break;
            case 6: whichx = 6; whichy = 0; break;
            case 7: whichx = 0; whichy = 0; break;
        }
        insert = new Graphic(16, 16, whichx*16, whichy*16, id);
        tiles(x,y).graphic.at(0) = (insert); // shoud just modify.. but w.e
    }

    if (x < width && y > 0) {// top right
        a = tiles(x,y-1).id == id; // top
        b = tiles(x+1,y-1).id == id; // top right
        c = tiles(x+1,y).id == id; // right
        switch (a + b*2 + c*4) {
            case 0: whichx = 9; whichy = 0; break;
            case 1: whichx = 7; whichy = 0; break;
            case 2: whichx = 9; whichy = 0; break;
            case 3: whichx = 7; whichy = 0; break;
            case 4: whichx = 5; whichy = 0; break;
            case 5: whichx = 3; whichy = 0; break;
            case 6: whichx = 5; whichy = 0; break;
            case 7: whichx = 1; whichy = 0; break;
        }
        insert = new Graphic(16, 16, whichx*16, whichy*16, id);
        tiles(x,y).graphic.at(1) = (insert); // shoud just modify.. but w.e
    }

    if (x > 0 && y < height) {// bottom left
        a = tiles(x,y+1).id == id; // bottom
        b = tiles(x-1,y+1).id == id; // bottom left
        c = tiles(x-1,y).id == id; // left
        switch (a + b*2 + c*4) {
            case 0: whichx = 8; whichy = 1; break;
            case 1: whichx = 6; whichy = 1; break;
            case 2: whichx = 8; whichy = 1; break;
            case 3: whichx = 6; whichy = 1; break;
            case 4: whichx = 4; whichy = 1; break;
            case 5: whichx = 2; whichy = 1; break;
            case 6: whichx = 4; whichy = 1; break;
            case 7: whichx = 0; whichy = 1; break;
        }
        insert = new Graphic(16, 16, whichx*16, whichy*16, id);
        tiles(x,y).graphic.at(2) = (insert); // shoud just modify.. but w.e
    }

    if (x < width && y < height) {// bottom right
        a = tiles(x+1,y).id == id; // right
        b = tiles(x+1,y+1).id == id; // bottom right
        c = tiles(x,y+1).id == id; // bottom
        switch (a + b*2 + c*4) {
            case 0: whichx = 9; whichy = 1; break;
            case 1: whichx = 5; whichy = 1; break;
            case 2: whichx = 9; whichy = 1; break;
            case 3: whichx = 5; whichy = 1; break;
            case 4: whichx = 7; whichy = 1; break;
            case 5: whichx = 3; whichy = 1; break;
            case 6: whichx = 7; whichy = 1; break;
            case 7: whichx = 1; whichy = 1; break;
        }
        insert = new Graphic(16, 16, whichx*16, whichy*16, id);
        tiles(x,y).graphic.at(3) = (insert); // shoud just modify.. but w.e
    }
}

void draw() {
    int dx = 0, dy = 0;
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            dx = x * 32;
            dy = y * 32;



            drawSprite(sprite[tiles(x,y).id], screen, tiles(x,y).graphic[0].srcx, tiles(x,y).graphic[0].srcy, dx   , dy   , 16, 16);
            printf("0>%d.%d,", tiles(x,y).graphic.at(0).srcx, tiles(x,y).graphic.at(0).srcy);// something wrong with this
            
            drawSprite(sprite[tiles(x,y).id], screen, tiles(x,y).graphic[1].srcx, tiles(x,y).graphic[1].srcy, dx+16, dy   , 16, 16);
            printf("1>%d.%d,", tiles(x,y).graphic.at(1).srcx, tiles(x,y).graphic.at(1).srcy); // and this
            
            drawSprite(sprite[tiles(x,y).id], screen, tiles(x,y).graphic[2].srcx, tiles(x,y).graphic[2].srcy, dx   , dy+16, 16, 16);
            printf("2>%d.%d,", tiles(x,y).graphic.at(2).srcx, tiles(x,y).graphic.at(2).srcy);
            drawSprite(sprite[tiles(x,y).id], screen, tiles(x,y).graphic[3].srcx, tiles(x,y).graphic[3].srcy, dx+16, dy+16, 16, 16);
            printf("3>%d.%d   \n", tiles(x,y).graphic.at(3).srcx, tiles(x,y).graphic.at(3).srcy);
        }
        // printf("\n");
    }
}

// Tile* parse_tile(char* tiledata) {
//     return NULL;
// }

void init_map(int id) {
    int x, y;
    printf("should resize to this size: %d\n", height*width);
    tiles.resize(height*width);
    for (y = 0; y < height; y++) {
        for (x = 0; x < width; x++) {

            // if (x > 0 && x < width - 1 && y > 0 && y < height - 1) {
            //     update_tile(x,y,id);
            // }

            std::vector<Graphic> graphics;
            Graphic* test1 = new Graphic(16, 16, 0, 0, id);
            Graphic* test2 = new Graphic(16, 16, 16, 0, id);
            Graphic* test3 = new Graphic(16, 16, 0, 16, id);
            Graphic* test4 = new Graphic(16, 16, 16, 16, id);

            graphics.push_back(*test1);
            graphics.push_back(*test2);
            graphics.push_back(*test3);
            graphics.push_back(*test4);

            Tile insert(id,x,y,graphics);
            tiles(x,y) = (insert); 
        }
    }
    printf("tiles shud be inited\n");
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

