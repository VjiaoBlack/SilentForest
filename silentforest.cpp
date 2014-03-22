#include "silentforest.h"



int main(int argc, char *argv[]) {
    
    /* Start up SDL */
    init("Silent Forest");
    
    // // Call the cleanup function when the program exits 
    // atexit(cleanup);

    /* Loop indefinitely for messages */

    menu_loop();
    while (running) {
        getInput();   

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
    while(1) {
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
                                /* Exit the program */
                                exit(0);
                                return;
                        }
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
    for (int y = 0; y < 15; y++) {
        for (int x = 0; x < 20; x++) {
            switch (grid[y][x].id) {
                case 1000: // grass
                    drawSprite(grid[y][x].graphic, screen, grid[y][x].srcx, grid[y][x].srcy, grid[y][x].gridx*32,grid[y][x].gridy*32,32,32);
                    break;
                case 1: // tree
                    drawSprite(grass, screen, 0,0,grid[y][x].gridx*32,grid[y][x].gridy*32,32,32); // placehodler
                    drawSprite(grid[y][x].graphic, screen, 0, 0, grid[y][x].gridx*32 - grid[y][x].width + 32, grid[y][x].gridy*32 - grid[y][x].height + 32, grid[y][x].width, grid[y][x].height);
                    break;
                case 2: // water
                    break;



            }
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

void getInput() {
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
                break;
            break;
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

void init(char *title) {
    /* Initialise SDL Video */
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("Could not initialize SDL: %s\n", SDL_GetError());
        exit(1);
    }
    
    /* Open a 640 x 480 screen */
    screen = SDL_SetVideoMode( SCREEN_WIDTH, SCREEN_HEIGHT, 0, SDL_HWSURFACE | SDL_DOUBLEBUF );
    if (screen == NULL) {
        printf("Couldn't set screen mode to 640 x 480: %s\n", SDL_GetError());
        exit(1);
    }
    
    /* Set the screen title */ // ???
    SDL_WM_SetCaption(title, "Silent Forest");


    running = 1, xoffset = 0, yoffset = 0, sx = 0, sy = 3, gridx = 5, gridy = 7;

    bitmap = SDL_LoadBMP("Chris.bmp");
    grass = SDL_LoadBMP("grass.bmp");
    font = SDL_LoadBMP("letters.bmp");
    tree = SDL_LoadBMP("tree.bmp");
    board = SDL_LoadBMP("board.bmp");
    intro = SDL_LoadBMP("intro.bmp");
    water = SDL_LoadBMP("water.bmp");
    SDL_SetColorKey( tree, SDL_SRCCOLORKEY, SDL_MapRGB(tree->format, 255, 0, 255) ); 
    SDL_SetColorKey( bitmap, SDL_SRCCOLORKEY, SDL_MapRGB(bitmap->format, 255, 0, 255) ); 


    //inits the fontdata;

    letter_data['A'] = (letter_t) {16, 0, 0}; //sadface all this hardcoding
    char counter;
    int i = 0;
    for (counter = 'A'; counter < 'I'; counter++) {
        letter_data[counter] = (letter_t) {16, 20*(counter - 'A'), 0};
    }
    letter_data['I'] = (letter_t) {12, 160, 0};
    for (counter = 'J'; counter < 'M'; counter++) {
        letter_data[counter] = (letter_t) {16, 20*(counter - 'A') - 4, 0};
    }
    letter_data['M'] = (letter_t) {20, 260, 0};
    for (counter = 'N'; counter < 'T'; counter++) {
        letter_data[counter] = (letter_t) {16, 20*(counter - 'A'), 0};
    }
    letter_data['T'] = (letter_t) {20, 380, 0};
    letter_data['U'] = (letter_t) {16, 404, 0};
    letter_data['V'] = (letter_t) {20, 420, 0};
    letter_data['W'] = (letter_t) {20, 448, 0};
    letter_data['X'] = (letter_t) {16, 460, 0};
    letter_data['Y'] = (letter_t) {20, 480, 0};
    letter_data['Z'] = (letter_t) {16, 500, 0};

    letter_data['a'] = (letter_t) {16, 0, 32};
    for (counter = 'b'; counter < 'i'; counter++) {
        letter_data[counter] = (letter_t) {12, 16*(counter-'a')+4, 32};
    }
    letter_data['i'] = (letter_t) {4, 16*('i'-'a') + 4, 32};
    letter_data['j'] = (letter_t) {8, 16*('j'-'a') - 4, 32};
    letter_data['k'] = (letter_t) {12, 16*('k'-'a') - 8, 32};
    letter_data['l'] = (letter_t) {4, 16*('l'-'a') - 8, 32}; //next is - 20
    letter_data['m'] = (letter_t) {20, 16*('m'-'a') - 16, 32}; //next is - 12
    letter_data['n'] = (letter_t) {12, 16*('n'-'a') - 8, 32};
    letter_data['o'] = (letter_t) {12, 16*('o'-'a') - 8, 32};
    letter_data['p'] = (letter_t) {12, 16*('p'-'a') - 8, 32};
    letter_data['q'] = (letter_t) {16, 16*('q'-'a') - 8, 32}; 
    letter_data['r'] = (letter_t) {12, 16*('r'-'a') - 4, 32}; 
    letter_data['s'] = (letter_t) {12, 16*('s'-'a') - 4, 32}; 
    letter_data['t'] = (letter_t) {12, 16*('t'-'a') - 4, 32};
    letter_data['u'] = (letter_t) {16, 16*('u'-'a') - 4, 32};
    letter_data['v'] = (letter_t) {12, 16*('v'-'a') - 0, 32};
    letter_data['w'] = (letter_t) {20, 16*('w'-'a') - 0, 32};
    letter_data['x'] = (letter_t) {12, 16*('x'-'a') + 8, 32}; 
    letter_data['y'] = (letter_t) {12, 16*('y'-'a') + 8, 32}; 
    letter_data['z'] = (letter_t) {12, 16*('z'-'a') + 8, 32};

    letter_data['0'] = (letter_t) {16,16*(0),64};
    letter_data['1'] = (letter_t) {12,16*(1)+4,64};
    letter_data['2'] = (letter_t) {16,16*(2)+4,64};
    letter_data['3'] = (letter_t) {16,16*(3)+8,64};
    letter_data['4'] = (letter_t) {12,16*(4)+12,64};
    letter_data['5'] = (letter_t) {12,16*(5)+12,64};
    letter_data['6'] = (letter_t) {12,16*(6)+12,64};
    letter_data['7'] = (letter_t) {12,16*(7)+12,64};
    letter_data['8'] = (letter_t) {16,16*(8)+12,64};
    letter_data['9'] = (letter_t) {16,16*(9)+16,64};

    letter_data[' '] = (letter_t) {12, 540, 0};
    letter_data['!'] = (letter_t) {4, 176, 64};
    letter_data['?'] = (letter_t) {16, 184, 64};
    letter_data['.'] = (letter_t) {4, 204, 64};
    letter_data[','] = (letter_t) {8, 212, 64};

    letter_data['>'] = (letter_t) {8, 468, 32};


    char tiles[15][20];
    char temp;

    FILE* fp = fopen("map.txt", "r");

    for (int y = 0; y < 15; y++) {
        for (int x = 0; x < 20; x++) {
            temp = fgetc(fp);
            if(temp == '\n'){
                temp = fgetc(fp);
            }
            if(feof(fp)){
                break;
            }
            tiles[y][x] = temp;
        }
    }

    for (int y = 0; y < 15; y++) { // loads tiles
        for (int x = 0; x < 20; x++) {
            switch (tiles[y][x]) {
                case 'g':
                    grid[y][x] = *new Tile(1000,x,y,grass, 32,32, 0, 0);
                    break;
                case 't':
                    grid[y][x] = *new Tile(1,x,y,tree, 96, 32, 0, 0);
                    break;
                case 'w':
                    grid[y][x] = *new Tile(2,x,y,water, 32, 32, 32, 64);
                    break;
                default:
                    grid[y][x] = *new Tile(1000,x,y,grass, 32, 32, 0, 0);
                    break;

            }
        }
    }
}

void cleanup() {
    /* Shut down SDL */
    
    SDL_Quit();
    SDL_FreeSurface(screen);
}

void drawText(char* input, int x, int y) {
    while (*input != '\0') {
        x += drawLetter(*input, x, y) + 4;
        if (x > 550) {
            x = 50;
            y += 32;
        }
        input++;
    }
}

int drawLetter(char letter, int x, int y) { // returns letter width
    int width = 0; 
    drawSprite(font, 
        screen,
        letter_data[letter].xpos, letter_data[letter].ypos, 
        x, y,
        letter_data[letter].width, 32);
    width = letter_data[letter].width;

    return width;
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
