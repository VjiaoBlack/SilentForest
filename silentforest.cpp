#include "silentforest.h"

int main(int argc, char *argv[]) {
    
    /* Start up SDL */
    init("Silent Forest");
    
    // // Call the cleanup function when the program exits 
    // atexit(cleanup);

    /* Loop indefinitely for messages */
    while (running) {
        getInput();   

        update();

        //draws the scene

        SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 0, 0, 0));
        draw();
        drawSprite(bitmap, screen, sx * 32, sy * 32, gridx * 32 + xoffset, gridy * 32 + yoffset, 32, 32);
        SDL_Flip(screen);

        /* Sleep briefly to stop sucking up all the CPU time */
        SDL_Delay(16);
    }
    SDL_FreeSurface(bitmap);
    /* Exit the program */
    exit(0);
}

void draw() {
    for (int y = 0; y < 15; y++) {
        for (int x = 0; x < 20; x++) {
            drawSprite(grass, screen, 0,0,x*32,y*32,32,32);
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
        gridy--;
        yoffset = 32;
        sy = 0;
    } else if (keysHeld[SDLK_DOWN] && (xoffset == 0) && (yoffset == 0)) {
        gridy++;
        yoffset = -32;
        sy = 2;
    } else if ( keysHeld[SDLK_LEFT]  && (xoffset == 0) && (yoffset == 0)) {
        gridx--;
        xoffset = 32;
        sy = 1;
    } else if ( keysHeld[SDLK_RIGHT]  && (xoffset == 0) && (yoffset == 0)) {
        gridx++;
        xoffset = -32;
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

    printf("delay: %d\n", delay);
    switch (delay / 10) {
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


    running = 1, xoffset = 0, yoffset = 0, sx = 0, sy = 3, gridx = 0, gridy = 0;

    bitmap = SDL_LoadBMP("Chris.bmp");
    SDL_SetColorKey( bitmap, SDL_SRCCOLORKEY, SDL_MapRGB(bitmap->format, 255, 0, 255) ); 
    grass = SDL_LoadBMP("grass.bmp");

}

void cleanup() {
    /* Shut down SDL */
    
    SDL_Quit();
    SDL_FreeSurface(screen);
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
