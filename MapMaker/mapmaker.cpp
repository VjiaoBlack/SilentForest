
#include "mapmaker.h"

// code starts ##################################


int main(int argc, char* argv[]) {
    int which = -1; // -1 is uninitialized, 0 is tiles, 1 is items, 2 is special
    int end = 0;
    int x, y, id;
    // char* pointer; // each pointer gets 10 chars. // what does this do again
    char test;
    FILE* mapfile;

    char* tag = (char*) malloc(sizeof(char) * 16);
    char* content = (char*) malloc(sizeof(char) * 16);

    char cmd[32];
    if (argc == 2) {
        if ( (mapfile = fopen(argv[1], "r")) ) {
            end = feof(mapfile);
            height = 10;
            width = 15;
            fscanf(mapfile, "h:%d w:%d b:{id:%d}\n", &height, &width, &base_tile_id);
            printf("scanned: height is %d, width is %d, base tile id is %d\n", height, width, base_tile_id);
            

            init_map(base_tile_id = 1);

            printf("initialized map with given constraints.\n");
            while (!end) {
                end = feof(mapfile);
                switch (which) {
                    case -1: // currently seeing what to scan into
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
                            change_tile(x,y,id);
                        } else {
                            which = -1;
                            printf("tile input syntax broken; tile input has ended.\n");
                        }
                        break;
                    case 1: // the items
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
                    case 2: // the special
                        // refer to the google doc!!!!!
                        if (fscanf(mapfile,"%d:%d{", &x, &y)) {
                            printf("found tile %d, %d to have special\n", x, y);
                            while(fscanf(mapfile,"%s:%s,",tag,content)) {
                                tiles(x,y).specials[tag] = content;
                            }
                            if (fscanf(mapfile,"%s:%s", tag, content)) {
                                tiles(x,y).specials[tag] = content;
                            }
                            which = -1;
                            printf("special input syntax broken; special input has ended.\n");
                        }   
                        break;
                }
            }
        } else {
            printf("File not found... making new file: '%s'\n", argv[1]);
            // mapfile = fopen(argv[1], "w");
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

    while (running) {
        get_input();  

        update();

        //draws the scene

        SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 0, 0, 0));
        draw();
        SDL_Rect rect = {0,600,1280,200};
        SDL_FillRect(screen, &rect, 0x0055FF);
        drawSprite(sprite[draw_id], screen, 0, 0, 50, 650, 32, 32);

        for(y = 0; y < height; y++) {
            for (x = 0; x < width; x++) {
                drawSprite(sprite[tiles(x,y).id], screen, 0, 0, 100+x*4, 650+y*4, 4, 4);
            }
        }
        if (selectedx > -1 && selectedy > -1) {
            edit_objects(selectedx, selectedy);
        }
        if (keysHeld[SDLK_o])
            drawText("selecting", 280, 690);
        SDL_Flip(screen);

         // Sleep briefly to stop sucking up all the CPU time 
       SDL_Delay(16);
    }
    SDL_FreeSurface(screen);
     // Exit the program 
    if (mapfile)
        fclose(mapfile);
    // mapfile = fopen(argv[1], "w");
    // printf("yayyy\n");
    // char* tofile = (char*) malloc(sizeof(char) * sizeof(data_to_string()));
    // sprintf(tofile, "%s", data_to_string());
    // fwrite(tofile, sizeof(char), sizeof(tofile), mapfile);
    // printf("%s\n", tofile);
    // free(tofile);
    // printf("whoo\n");
    // fflush(mapfile);
    // fclose(mapfile);

    
    std::string tofile;
    tofile = data_to_string();
    std::ofstream out(argv[1]);
    out << tofile;
    out.close();
    
    SDL_Quit();
    exit(0);
    return 0;
}

void load_graphics() {
    // somehow put input graphics data here! yay! wow! smart!!!
    // note how most of this is currently in main().

}

void init() {

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
        if (keysHeld[SDLK_o]) {
            selectedx = (mousex-xoffset)/32; 
            selectedy = (mousey-yoffset)/32;
        }
        else if (tiles((mousex-xoffset)/32,(mousey-yoffset)/32).id != draw_id)
            change_tile((mousex-xoffset)/32, (mousey-yoffset)/32, draw_id);
    }
}

void change_tile(int x, int y, int id) {
    // note that the tiles are aligned; the 'id' is at the center of the tile.

    tiles(x,y).id = id;

    update_tile(x,y, id); // TODO: udpate neighbors
    if (x<width-1) {
        if (y<height-1) update_tile(x+1,y+1,tiles(x+1,y+1).id);
        if (y>0) update_tile(x+1,y-1,tiles(x+1,y-1).id);    
        update_tile(x+1,y  ,tiles(x+1,y  ).id);
    } 
    if (x>0) {
        if (y<height-1) update_tile(x-1,y+1,tiles(x-1,y+1).id);
        if (y>0) update_tile(x-1,y-1,tiles(x-1,y-1).id);
        update_tile(x-1,y  ,tiles(x-1,y  ).id);
    } 
    if (y<height-1)update_tile(x  ,y+1,tiles(x  ,y+1).id);
    if (y>0) update_tile(x  ,y-1,tiles(x  ,y-1).id);
    update_tile(x  ,y  ,tiles(x  ,y  ).id);
}

void update_tile(int x, int y, int id) {
    // doesnt change id, just updates tile.
    int whichx = 0, whichy = 0; // which complete tile to take minitiles from
    int a, b, c; // is neighbor id equal to urs?
    Graphic insert;


    if (id == 1) {
        insert = new Graphic(16, 16, 0, 0, id);
        tiles(x,y).graphic.at(0) = (insert);
        insert = new Graphic(16, 16, 16, 0, id);
        tiles(x,y).graphic.at(1) = (insert);
        insert = new Graphic(16, 16, 0, 16, id);
        tiles(x,y).graphic.at(2) = (insert);
        insert = new Graphic(16, 16, 16, 16, id);
        tiles(x,y).graphic.at(3) = (insert);

        return;
    }

    // note: this function needs to be shortened

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

    if (x < width-1 && y > 0) {// top right
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

    if (x > 0 && y < height-1) {// bottom left
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

    if (x < width-1 && y < height-1) {// bottom right
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



            drawSprite(sprite[tiles(x,y).id], screen, tiles(x,y).graphic[0].srcx, tiles(x,y).graphic[0].srcy, dx   +xoffset, dy   +yoffset, 16, 16);
            // printf("0>%d.%d,", tiles(x,y).graphic.at(0).srcx, tiles(x,y).graphic.at(0).srcy);// something wrong with this
            
            drawSprite(sprite[tiles(x,y).id], screen, tiles(x,y).graphic[1].srcx, tiles(x,y).graphic[1].srcy, dx+16+xoffset, dy   +yoffset, 16, 16);
            // printf("1>%d.%d,", tiles(x,y).graphic.at(1).srcx, tiles(x,y).graphic.at(1).srcy); // and this
            
            drawSprite(sprite[tiles(x,y).id], screen, tiles(x,y).graphic[2].srcx, tiles(x,y).graphic[2].srcy, dx   +xoffset, dy+16+yoffset, 16, 16);
            // printf("2>%d.%d,", tiles(x,y).graphic.at(2).srcx, tiles(x,y).graphic.at(2).srcy);
            drawSprite(sprite[tiles(x,y).id], screen, tiles(x,y).graphic[3].srcx, tiles(x,y).graphic[3].srcy, dx+16+xoffset, dy+16+yoffset, 16, 16);
            // printf("3>%d.%d   \n", tiles(x,y).graphic.at(3).srcx, tiles(x,y).graphic.at(3).srcy);
        }
        // printf("\n");
    }
}

std::string parse_tile(Tile tile) {
    std::string output;
    char* tostring = (char*) malloc(sizeof(char) * 64);
    sprintf(tostring,"{pos:%d;%d|id:%d}", tile.gridx, tile.gridy, tile.id);
    output.append(tostring);
    free(tostring);
    return output;
}

std::string parse_item(Tile tile) {
    std::string output;
    char* itemid = (char*) malloc(sizeof(char) * 8);
    char* tostring = (char*) malloc(sizeof(char) * 64);
    sprintf(tostring,"%d;%d:{",tile.gridx,tile.gridy);
    output.append(tostring);
    std::list<Object>::iterator it=tile.objects.begin();
    while (it != tile.objects.end()) {
        sprintf(itemid,"%d",it->id);
        output.append(itemid);
        if (++it != tile.objects.end()) {
            output.append(",");
        }
    }
    output.append("}");
    free(tostring);
    free(itemid);
    return output;
}

void init_map(int id) {
    int x, y;
    draw_id = 1;
    selectedx = -1;
    selectedy = -1;
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

std::string data_to_string() {
    char* tostring = (char*) malloc(sizeof(char) * 64);
    sprintf(tostring,"h:%d w:%d b:{id:%d}\n", height, width, base_tile_id); // error!!

    std::string output;
    output.append(tostring);
    output.append("tiles[\n");
    for(int y = 0; y < height; y++) {
        for(int x = 0; x < width; x++) {
            if (tiles(x,y).id != base_tile_id) {
                output.append(parse_tile(tiles(x,y)));
                output.append("\n");
            }
        }
    }
    output.append("]\n");
    output.append("items[\n");
    for(int y = 0; y < height; y++) {
        for(int x = 0; x < width; x++) {
            if (tiles(x,y).objects.size() > 0) {
                output.append(parse_item(tiles(x,y)));
                output.append("\n");
            }
        }
    }
    output.append("]\nend");
    return output;
}

void drawSprite(SDL_Surface* imageSurface, SDL_Surface* screenSurface,int sx, int sy, int dx, int dy,int w, int h) {
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

void drawText(char* input, int x, int y) {
    while (*input != '\0') {
        x += drawLetter(*input, x, y) + 4;
        if (x > 700) {
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

void edit_objects(int grx, int gry) {
    char* buffer = (char*) malloc(sizeof(char) * 32);
    sprintf(buffer, "Editing Tile at %d, %d.", grx, gry);
    drawText(buffer, 200, 650);

    char* idbuf = (char*) malloc(sizeof(char) * 8);
    int x = 570, y = 650;
    // print out all the objects here
    // object.id
    if (tiles(grx,gry).objects.size() > 0) {
        for (std::list<Object>::iterator it=tiles(grx,gry).objects.begin(); it != tiles(grx,gry).objects.end(); ++it) {
            sprintf(idbuf, "%d", it->id);
            drawText(idbuf, x, y);
            y+= 32;
            if (y > 770) {
                x += 80;
            }
        }

    } else {
        drawText("no items", 570, 650);
    }
    //



}

