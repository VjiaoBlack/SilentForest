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
    
    /* Open a 1280 x 800 screen */
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
if (delay < 1) 
        delay = 40;
    if ( keysHeld[SDLK_ESCAPE] ) {
        running = false;
    }
    if ( keysHeld[SDLK_UP] && (xoffset == 0) && (yoffset == 0)) {
        if (grid[gridy - 1][gridx].id / 1000 == 1 && gridy > 0) {
            gridy--;
            yoffset = 32;
        }
        sy = 0;
    } else if (keysHeld[SDLK_DOWN] && (xoffset == 0) && (yoffset == 0)) {
        if (grid[gridy + 1][gridx].id / 1000 == 1 && gridy < 15) {
            gridy++;
            yoffset = -32;
        }
        sy = 2;
    } else if ( keysHeld[SDLK_LEFT]  && (xoffset == 0) && (yoffset == 0) ) {
        if (grid[gridy][gridx - 1].id / 1000 == 1 && gridx > 0) {
            gridx--;
            xoffset = 32;
        }
        sy = 1;
    } else if ( keysHeld[SDLK_RIGHT]  && (xoffset == 0) && (yoffset == 0) ) {
        if (grid[gridy][gridx + 1].id / 1000 == 1 && gridx < 20) {
            gridx++;
            xoffset = -32;
        }
        sy = 3;
    } else if (xoffset == 0 && yoffset == 0) {
        delay = 0;
        sx = 0;
    }

    if (xoffset > 0) {
        xoffset -= 2;
        delay--;
    }
    if (xoffset < 0) {
        xoffset += 2;
        delay--;
    }
    if (yoffset > 0) {
        yoffset -= 2;
        delay--;
    }
    if (yoffset < 0) {
        yoffset += 2;
        delay--;
    }

    switch (delay / 10 ) {
        case 0:
        case 2:
            sx = 0;
            break;
        case 1:
            sx = 1;
            break;
        case 3: 
            sx = 2;
            break;
    }
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

}



