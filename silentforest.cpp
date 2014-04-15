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

    // main menu
    menu_loop();

    while (running) {
        get_input();  

        respond_to_input();

        update();



        //draws the scene

        SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 0, 0, 0));
        draw();
        SDL_Flip(screen);

        /* Sleep briefly to stop sucking up all the CPU time */
        SDL_Delay(1);
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
    drawSprite(intro, screen, 0, 0, 320, 300, 640, 480);
    drawSprite(board, screen, 0, 0, 520, 500, 240, 160);

    drawText("Silent Forest", 540, 510);
    drawText("Play", 580, 560);
    drawText("Exit", 580, 600);



    switch (choice) {
        case 0:
            drawText(">", 560, 560);
            break;
        case 1:
            drawText(">", 560, 600);
            break;
    }

    SDL_Flip(screen);
}


void menu_loop() {
    SDL_Event event;
    int choice = 0;
    int running = 1;
    while(running) {
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
                        choice = 1;
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

void respond_to_input() {
    if (keysHeld[SDLK_i]) {
        inventory_loop();
        keysHeld[SDLK_i] = 0;
    }
}

void inventory_loop() {

    int running = 1;
    SDL_Event event;

    while(running) {
        if (SDL_PollEvent(&event)) {
            if (event.type == SDL_KEYDOWN) {
                switch (event.key.keysym.sym) {
                    case SDLK_ESCAPE:
                    case SDLK_q:    
                        running = 0;
                        break;
                    default:      
                        break;
                }
            }
        }
        SDL_Delay(16);
        draw_inventory();

    }


}

void draw_inventory() {


    SDL_Rect bkgrd = {0,0,1280, 800};

    SDL_FillRect(screen, &bkgrd, 0xCCCCCC);

    drawText("Inventory", 7 * 32, SCREEN_HEIGHT - 4 * 32);

    int a = 100;
    int b = 200;
    int pos = 0;
    stringstream idstrm;



    SDL_Rect square = {a,b,30,30};

    for (int y = 0; y < 10; y++) {
        for (int x = 0; x < 10; x++) {
            pos = y * 10 + x;
            square.x = a + 32 * x;
            SDL_FillRect(screen, &square, 0xAAAAAA);

        }
        square.y = b + 32 * y;
    }

    for (int y = 0; y < 10; y++) {
        for (int x = 0; x < 10; x++) {
            pos = y * 10 + x;

            if (pos < (int) inventory.size()) {
                idstrm.str(std::string());
                idstrm << tiles(gridx,gridy).objects[pos].id;
                drawText(idstrm.str(), a + 32 * x, b + 32 * y);
            }
        }
    }
    SDL_Flip(screen);
}


void draw() {

    draw_map();
    draw_character();
    draw_stats();
    draw_debug();
}

void draw_debug() {

}

void draw_stats() {
    SDL_Rect rect = {0,SCREEN_HEIGHT - 5 * 32,SCREEN_WIDTH,5 * 32};
    SDL_FillRect(screen, &rect, 0x0055FF);

    drawText("HP>", 2 * 32, SCREEN_HEIGHT - 4 * 32);
    drawText("MP>", 2 * 32, SCREEN_HEIGHT - 3 * 32);

    drawText("15", 4 * 32, SCREEN_HEIGHT - 4 * 32);
    drawText("2", 4 * 32, SCREEN_HEIGHT - 3 * 32);


   
}

void draw_map() {


    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            draw_tile(x,y);


        }
    }
}

void draw_tile(int grx, int gry) {

    // TODO: Implement drawing of tiles that are bigger than 1 unit.

    int dx = 0, dy = 0;
    dx = grx * 32;
    dy = gry * 32;
    drawSprite(sprite[tiles(grx,gry).id], screen, tiles(grx,gry).graphic.at(0).srcx, tiles(grx,gry).graphic.at(0).srcy, dx   +xmapoffset, dy   +ymapoffset, 16, 16);
    drawSprite(sprite[tiles(grx,gry).id], screen, tiles(grx,gry).graphic.at(1).srcx, tiles(grx,gry).graphic.at(1).srcy, dx+16+xmapoffset, dy   +ymapoffset, 16, 16);
    drawSprite(sprite[tiles(grx,gry).id], screen, tiles(grx,gry).graphic.at(2).srcx, tiles(grx,gry).graphic.at(2).srcy, dx   +xmapoffset, dy+16+ymapoffset, 16, 16);
    drawSprite(sprite[tiles(grx,gry).id], screen, tiles(grx,gry).graphic.at(3).srcx, tiles(grx,gry).graphic.at(3).srcy, dx+16+xmapoffset, dy+16+ymapoffset, 16, 16);

    for (std::vector<Object>::iterator it = tiles(grx,gry).objects.begin(); it != tiles(grx,gry).objects.end(); it++) {
        drawSprite(chest, screen, 0, 0, dx + xmapoffset, dy + ymapoffset, 32, 32);
    }

}

void draw_character() {
    drawSprite(bitmap, screen, sx * 32, sy * 32, gridx * 32 + xcharoffset + xmapoffset, gridy * 32 + ycharoffset + ymapoffset, 32, 32); // draws char.

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
                break;
            break;
        }
    }
}

void update() {
    if (delay < 1) 
        delay = 32;
    if ( keysHeld[SDLK_ESCAPE] ) {
        running = false;
    }


    // following moves char's grid pos if is prefectly aligned.
    if ( keysHeld[SDLK_UP] && (xcharoffset == 0) && (ycharoffset == 0)) {
        if ( (gridy > 0) && tiles(gridx,gridy-1).id / 1000 == 0) {
            gridy--;
            ycharoffset = 32;
        }
        sy = 0;
    } else if (keysHeld[SDLK_DOWN] && (xcharoffset == 0) && (ycharoffset == 0)) {
        if ( (gridy<height-1) && tiles(gridx,gridy+1).id / 1000 == 0) {
            gridy++;
            ycharoffset = -32;
        }
        sy = 2;
    } else if ( keysHeld[SDLK_LEFT]  && (xcharoffset == 0) && (ycharoffset == 0) ) {
        if ( (gridx>0) && tiles(gridx-1,gridy).id / 1000 == 0) {
            gridx--;
            xcharoffset = 32;
        }
        sy = 1;
    } else if ( keysHeld[SDLK_RIGHT]  && (xcharoffset == 0) && (ycharoffset == 0) ) {
        if ( (gridx<width-1) && tiles(gridx+1,gridy).id / 1000 == 0) {
            gridx++;
            xcharoffset = -32;
        }
        sy = 3;
    } else if (xcharoffset == 0 && ycharoffset == 0) {
        delay = 0;
        sx = 0;
    }




    if (xcharoffset > 0) { // to the left
        xcharoffset -= 4;
        delay--;
    }
    if (xcharoffset < 0) { // to the right
        xcharoffset += 4;
        delay--;
    }
    if (ycharoffset > 0) { // to the top
        ycharoffset -= 4;
        delay--;
    }
    if (ycharoffset < 0) { // to the bottom
        ycharoffset += 4;
        delay--;
    }


    // the following codes for the effect where the view follows the camera
    int charx = gridx * 32;
    int chary = gridy * 32;
    int scrnheight = SCREEN_HEIGHT / 32 - 5; // -5 for the stats menu
    int scrnwidth = SCREEN_WIDTH / 32;

    if ( charx >= (scrnwidth / 2) * 32   &&   charx <= width * 32 - (scrnwidth / 2) * 32) {
        if (xcharoffset >= 0 && gridx < width - scrnwidth / 2) {
            xmapoffset = 0 - charx - xcharoffset + (scrnwidth / 2) * 32;
        }
        else if (xcharoffset <= 0 && gridx > scrnwidth / 2 ) {
            xmapoffset = 0 - charx - xcharoffset + (scrnwidth / 2) * 32;
        }
    }  

    if ( chary >= (scrnheight / 2) * 32  &&   chary <= height * 32 - (scrnheight / 2) * 32) {
        if (ycharoffset >= 0 && gridy < height - scrnheight / 2) {
            ymapoffset = 0 - chary - ycharoffset + (scrnheight / 2) * 32; // UGH TODO
        }
        else if (ycharoffset <= 0 && gridy > scrnheight / 2) { // going down
            ymapoffset = 0 - chary - ycharoffset + (scrnheight / 2) * 32;
        }
    } 



    // viewing objects

    if ( keysHeld[SDLK_SPACE] ) {
        // picks up stuff
        if ((int) tiles(gridx,gridy).objects.size() > 0) {
            inventory.push_back(tiles(gridx,gridy).objects[ tiles(gridx,gridy).objects.size() - 1 ] );
            tiles(gridx,gridy).objects.pop_back();
            keysHeld[SDLK_SPACE] = 0;
        }

    }


    switch ((delay / 4) % 4 ) {
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

    running = 1, xcharoffset = 0, ycharoffset = 0, sx = 0, sy = 3, gridx = 5, gridy = 7;
    xmapoffset = 0, ymapoffset = 0;

    init_font();
    bitmap = SDL_LoadBMP("Chris.bmp");
    grass = SDL_LoadBMP("grass.bmp");
    font = SDL_LoadBMP("letters.bmp");
    tree = SDL_LoadBMP("tree.bmp");
    board = SDL_LoadBMP("board.bmp");
    intro = SDL_LoadBMP("intro.bmp");
    water = SDL_LoadBMP("water.bmp");
    chest = SDL_LoadBMP("chest.bmp");

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
    sprite[0] = grass;
    sprite[1010] = water;
}

void cleanup() {
    /* Shut down SDL */
    
    SDL_Quit();
    SDL_FreeSurface(screen);
}

