#include "font.h"
#include "mapio.h"
#include "silentforest.h"



int main(int argc, char *argv[]) {
    FILE* mapfile;
    if (!(mapfile = parse_input(argc, argv))) {
        return -1;
    }
    // setting up map and stuff
    init();
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            update_tile(x,y,tiles(x,y).id);
        }
    }

// 
    // main menu
    menu_loop();

    while (running) {
        get_input();   

        update();

        //draws the scene

        SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 0, 0, 0));
        draw();
        SDL_Flip(screen);

        /* Sleep briefly to stop sucking up all the CPU time */
        SDL_Delay(16);
    }
    SDL_FreeSurface(bitmap);
    SDL_FreeSurface(tree);
    SDL_FreeSurface(grass);
    SDL_FreeSurface(font);
    /* Exit the program */

    if (mapfile)
        fclose(mapfile);
    
    save_map((argv[1]));
    
    SDL_Quit();
    exit(0);
    return 0;
    exit(0);
}

void drawmenu(int choice) {
    drawSprite(intro, screen, 0, 0, 0, 0, 640, 480);
    drawSprite(board, screen, 0, 0, 200, 200, 240, 160);

    drawText("Silent Forest", 220, 210);
    drawText("Play", 260, 260);
    drawText("Exit", 260, 300);



    switch (choice) {
        case 0:
            drawText(">", 240, 260);
            break;
        case 1:
            drawText(">", 240, 300);
            break;
    }

    SDL_Flip(screen);
}


void menu_loop() {
    SDL_Event event;
    int choice = 0;
    int running = 1;
    while(running) {
        // i should draw something here.
        if (SDL_PollEvent(&event)) {
            if (event.type == SDL_KEYDOWN) {
                switch (event.key.keysym.sym) {
                    case SDLK_UP:
                        choice = !choice;
                        break;
                    case SDLK_DOWN:
                        choice = !choice;
                        break;
                    case SDLK_KP_ENTER:
                    case SDLK_RETURN:
                        switch (choice) {
                            case 0:
                                return;
                            case 1:
                                SDL_FreeSurface(bitmap);
                                SDL_FreeSurface(tree);
                                SDL_FreeSurface(grass);
                                SDL_FreeSurface(font);
                                SDL_FreeSurface(intro);
                                running = false;
                                /* Exit the program */
                                exit(0);
                                return;
                        }
                        break;
                    case SDLK_ESCAPE:
                    case SDLK_q:    
                        running = 0;
                        break;
                    default:      
                        break;
                }
            }
        }
        drawmenu(choice);
        SDL_Delay(16);
    }
}


void draw() {

    draw_map();
    draw_character();


    // for (int y = 0; y < 15; y++) {
    //     for (int x = 0; x < 20; x++) {
    //         switch (grid[y][x].id) {
    //             case 1000: // grass
    //                 drawSprite(grid[y][x].graphic, screen, grid[y][x].srcx, grid[y][x].srcy, grid[y][x].gridx*32,grid[y][x].gridy*32,32,32);
    //                 break;
    //             case 1: // tree
    //                 drawSprite(grass, screen, 0,0,grid[y][x].gridx*32,grid[y][x].gridy*32,32,32); // placehodler
    //                 drawSprite(grid[y][x].graphic, screen, 0, 0, grid[y][x].gridx*32 - grid[y][x].width + 32, grid[y][x].gridy*32 - grid[y][x].height + 32, grid[y][x].width, grid[y][x].height);
    //                 break;
    //             case 2: // water
    //                 break;



    //         }
    //         if ((x == gridx && y == gridy) ||
    //             (xoffset > 0 && gridx + 1 == x && y == gridy) || 
    //             (yoffset > 0 && gridy + 1 == y && x == gridx)) {
    //             drawSprite(bitmap, screen, sx * 32, sy * 32, gridx * 32 + xoffset, gridy * 32 + yoffset, 32, 32); // draws char.
    //             // NOTE: must implement the walking-on-gridlines algorithm
    //                 // MUST MUST MUST IMPLEMENT :((
    //         }
    //     }
    // }
}

void draw_map() {
    int dx = 0, dy = 0;
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            dx = x * 32;
            dy = y * 32;
            drawSprite(sprite[tiles(x,y).id], screen, tiles(x,y).graphic.at(0).srcx, tiles(x,y).graphic.at(0).srcy, dx   /*+xoffset*/, dy   /*+yoffset*/, 16, 16);
            drawSprite(sprite[tiles(x,y).id], screen, tiles(x,y).graphic.at(1).srcx, tiles(x,y).graphic.at(1).srcy, dx+16/*+xoffset*/, dy   /*+yoffset*/, 16, 16);
            drawSprite(sprite[tiles(x,y).id], screen, tiles(x,y).graphic.at(2).srcx, tiles(x,y).graphic.at(2).srcy, dx   /*+xoffset*/, dy+16/*+yoffset*/, 16, 16);
            drawSprite(sprite[tiles(x,y).id], screen, tiles(x,y).graphic.at(3).srcx, tiles(x,y).graphic.at(3).srcy, dx+16/*+xoffset*/, dy+16/*+yoffset*/, 16, 16);
            if ((x == gridx && y == gridy) ||
                (xoffset > 0 && gridx + 1 == x && y == gridy) || 
                (yoffset > 0 && gridy + 1 == y && x == gridx)) {
                drawSprite(bitmap, screen, sx * 32, sy * 32, gridx * 32 + xoffset, gridy * 32 + yoffset, 32, 32); // draws char.
                // NOTE: must implement the walking-on-gridlines algorithm
                    // MUST MUST MUST IMPLEMENT :((
            }


        }
    }
}

void draw_character() {
    // for (int x = 0; x < width; x++) {
    //     for (int y = 0; y < height; y++) {
    //         if ((x == gridx && y == gridy) ||
    //             (xoffset > 0 && gridx + 1 == x && y == gridy) || 
    //             (yoffset > 0 && gridy + 1 == y && x == gridx)) {
    //             drawSprite(bitmap, screen, sx * 32, sy * 32, gridx * 32 + xoffset, gridy * 32 + yoffset, 32, 32); // draws char.
    //             // NOTE: must implement the walking-on-gridlines algorithm
    //                 // MUST MUST MUST IMPLEMENT :((
    //         }
    //     }
    // }
}

void get_input() {
    SDL_Event event;
    
    /* Loop through waiting messages and process them */
    if (SDL_PollEvent(&event)) {

        switch (event.type) {
            /* Closing the Window or pressing Escape will exit the program */
            case SDL_QUIT:
                exit(0);
                break;
            case SDL_KEYUP:
                keysHeld[event.key.keysym.sym] = false; 
                break;
            case SDL_KEYDOWN:
                keysHeld[event.key.keysym.sym] = true;
                printf("asdf\n");
                break;
            break;
        }
    }
}

void update() {
    int y = 0; 
    int x = 0;
    if (delay < 1) 
        delay = 40;
    if ( keysHeld[SDLK_ESCAPE] ) {
        running = false;
    }
    if ( keysHeld[SDLK_UP] && (xoffset == 0) && (yoffset == 0)) {
        // if (tiles(x,y-1).id / 1000 == 1 && gridy > 0) {
            gridy--;
            yoffset = 32;
        // }
        sy = 0;
    } else if (keysHeld[SDLK_DOWN] && (xoffset == 0) && (yoffset == 0)) {
        // if (tiles(x,y+1).id / 1000 == 1 && gridy < 15) {
            gridy++;
            yoffset = -32;
        // }
        sy = 2;
    } else if ( keysHeld[SDLK_LEFT]  && (xoffset == 0) && (yoffset == 0) ) {
        // if (tiles(x-1,y).id / 1000 == 1 && gridx > 0) {
            gridx--;
            xoffset = 32;
        // }
        sy = 1;
    } else if ( keysHeld[SDLK_RIGHT]  && (xoffset == 0) && (yoffset == 0) ) {
        // if (tiles(x+1,y).id / 1000 == 1 && gridx < 20) {
            gridx++;
            xoffset = -32;
        // }
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

void init() {
    /* Initialise SDL Video */
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("Could not initialize SDL: %s\n", SDL_GetError());
        exit(1);
    }
    /* Open a 1280 x 800 screen */
    screen = SDL_SetVideoMode( SCREEN_WIDTH, SCREEN_HEIGHT, 0, SDL_HWSURFACE | SDL_DOUBLEBUF );
    if (screen == NULL) {
        printf("Couldn't set screen mode to 640 x 480: %s\n", SDL_GetError());
        exit(1);
    }
    
    // /* Set the screen title */ // ???
    // SDL_WM_SetCaption(title, "Silent Forest");

    running = 1, xoffset = 0, yoffset = 0, sx = 0, sy = 3, gridx = 5, gridy = 7;
    init_font();
    bitmap = SDL_LoadBMP("Chris.bmp");
    grass = SDL_LoadBMP("grass.bmp");
    font = SDL_LoadBMP("letters.bmp");
    tree = SDL_LoadBMP("tree.bmp");
    board = SDL_LoadBMP("board.bmp");
    intro = SDL_LoadBMP("intro.bmp");
    water = SDL_LoadBMP("water.bmp");

    if (tree)
        SDL_SetColorKey( tree, SDL_SRCCOLORKEY, SDL_MapRGB(tree->format, 255, 0, 255) ); 

    if (bitmap)
        SDL_SetColorKey( bitmap, SDL_SRCCOLORKEY, SDL_MapRGB(bitmap->format, 255, 0, 255) ); 

    if (font) 
        SDL_SetColorKey( font, SDL_SRCCOLORKEY, SDL_MapRGB(font->format, 255, 0, 255) ); 

    // puts graphics (sprites) into a vector
    load_graphics(); 



}

void load_graphics() {
    sprite[1] = grass;
    sprite[2] = water;
}

void cleanup() {
    /* Shut down SDL */
    
    SDL_Quit();
    SDL_FreeSurface(screen);
}

