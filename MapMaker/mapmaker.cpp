#include "font.h"
#include "mapio.h"
#include "mapmaker.h"

// code starts ##################################


int main(int argc, char* argv[]) {
    int which = -1; // -1 is uninitialized, 0 is tiles, 1 is items, 2 is special
    int end = 0;
    int x, y, id;
    int i = 0;
    // char* pointer; // each pointer gets 10 chars. // what does this do again
    FILE* mapfile = NULL;
    
    if (!(mapfile = parse_input(argc, argv))) {
        return -1;
    }

    init();
    for (y = 0; y < height; y++) {
        for (x = 0; x < width; x++) {
            update_tile(x,y,tiles(x,y).id);
        }
    }
    while (running) {
        get_input();  
        update();

        //draws the scene
        SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 0, 0, 0));
        draw();

        if (selectedx > -1 && selectedy > -1) { // if the user has selected a tile
            switch (which_disp) {   // then decide what to do, depending on which_disp
                case 0: 
                    edit_objects(selectedx, selectedy);
                    break;
                case 1:
                    edit_specials(selectedx, selectedy);
                    break;
                default: 
                    break;   
            }
        }

        SDL_Flip(screen);

         // Sleep briefly to stop sucking up all the CPU time 
       SDL_Delay(16);
    }
    SDL_FreeSurface(screen);


    // Exit the program 
    if (mapfile)
        fclose(mapfile);
    
    save_map((argv[1]));
    
    SDL_Quit();
    exit(0);
    return 0;
}

void load_graphics() {
    sprite[1] = grass;
    sprite[2] = water;
}

void init() {
    which_disp = -1;
    selectedx = -1;
    selectedy = -1;
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("Could not initialize SDL: %s\n", SDL_GetError());
        exit(1);
    }
    
    /* Open a 640 x 480 screen */
    screen = SDL_SetVideoMode( 1280, 800, 0, SDL_HWSURFACE | SDL_DOUBLEBUF );
    grass = SDL_LoadBMP("grass.bmp");
    water = SDL_LoadBMP("water.bmp");
        font = SDL_LoadBMP("letters.bmp");
    SDL_SetColorKey( font, SDL_SRCCOLORKEY, SDL_MapRGB(font->format, 255, 0, 255) );

    init_font();

    load_graphics();

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
            case SDL_MOUSEMOTION:
                mousex = event.motion.x;
                mousey = event.motion.y;
                break;
            case SDL_MOUSEBUTTONDOWN:
                switch (event.button.button) {
                    case SDL_BUTTON_LEFT:
                        mouseleftdown = true;
                        break;
                }
                break;
            case SDL_MOUSEBUTTONUP:
                switch (event.button.button) {
                    case SDL_BUTTON_LEFT:
                        mouseleftdown = false;
                        break;
                }
        }
    }
}

void update() {
    if ( keysHeld[SDLK_ESCAPE] ) {
        running = false;
    }
    if (keysHeld[SDLK_w]){
        draw_id = 2;
    }
    if (keysHeld[SDLK_g]){
        draw_id = 1;
    }
    if (keysHeld[SDLK_UP]){
        if (keysHeld[SDLK_LSHIFT])
            yoffset += 16;
        else
            yoffset+=3;
    }
    if (keysHeld[SDLK_DOWN]){
        if (keysHeld[SDLK_LSHIFT])
            yoffset -= 16;
        else
            yoffset-=3;
    }
    if (keysHeld[SDLK_LEFT]){
        if (keysHeld[SDLK_LSHIFT])
            xoffset += 16;
        else
            xoffset+=3;
    }
    if (keysHeld[SDLK_RIGHT]){
        if (keysHeld[SDLK_LSHIFT])
            xoffset -= 16;
        else
            xoffset-=3;
    }
    if (mouseleftdown && mousex > xoffset && mousey > yoffset && mousex < width*32 && mousey < height*32) {
        if (keysHeld[SDLK_o] || keysHeld[SDLK_s]) {
            selectedx = (mousex-xoffset)/32; 
            selectedy = (mousey-yoffset)/32;
        }
        else if (tiles((mousex-xoffset)/32,(mousey-yoffset)/32).id != draw_id)
            change_tile((mousex-xoffset)/32, (mousey-yoffset)/32, draw_id);
    }
    if (keysHeld[SDLK_o]) 
        which_disp = 0;
    if (keysHeld[SDLK_s])
        which_disp = 1;
}

void draw(){
    draw_map();
    draw_menu();
}

void draw_map() {
    int dx = 0, dy = 0;
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            dx = x * 32;
            dy = y * 32;
            drawSprite(sprite[tiles(x,y).id], screen, tiles(x,y).graphic.at(0).srcx, tiles(x,y).graphic.at(0).srcy, dx   +xoffset, dy   +yoffset, 16, 16);
            drawSprite(sprite[tiles(x,y).id], screen, tiles(x,y).graphic.at(1).srcx, tiles(x,y).graphic.at(1).srcy, dx+16+xoffset, dy   +yoffset, 16, 16);
            drawSprite(sprite[tiles(x,y).id], screen, tiles(x,y).graphic.at(2).srcx, tiles(x,y).graphic.at(2).srcy, dx   +xoffset, dy+16+yoffset, 16, 16);
            drawSprite(sprite[tiles(x,y).id], screen, tiles(x,y).graphic.at(3).srcx, tiles(x,y).graphic.at(3).srcy, dx+16+xoffset, dy+16+yoffset, 16, 16);
        }
    }
}

void draw_menu() {
        SDL_Rect rect = {0,600,1280,200};
        SDL_FillRect(screen, &rect, 0x0055FF);
        drawSprite(sprite[draw_id], screen, 0, 0, 50, 650, 32, 32);

        // this draws minimap
        for(int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                drawSprite(sprite[tiles(x,y).id], screen, 0, 0, 100+x*4, 650+y*4, 4, 4);
            }
        }

        // indicator for what is currently being edited.
        if (keysHeld[SDLK_o])  
            drawText("object", 280, 690);
        if (keysHeld[SDLK_s])
            drawText("specials", 280, 690);
}



