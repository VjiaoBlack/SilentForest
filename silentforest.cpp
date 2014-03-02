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
        drawSprite(bitmap, screen, sx * 32, sy * 32, x, y, 32, 32);
        SDL_Flip(screen);

        /* Sleep briefly to stop sucking up all the CPU time */
        SDL_Delay(16);
    }
    SDL_FreeSurface(bitmap);
    /* Exit the program */
    exit(0);
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
    if (!delay) 
        delay = 40;
    if ( keysHeld[SDLK_ESCAPE] ) {
        running = false;
    }
    if ( keysHeld[SDLK_UP] ) {
        y -= 2;
        sy = 0;
        delay--;
    } else
    if (keysHeld[SDLK_DOWN]) {
        y += 2;
        sy = 2;
        delay--;
    } else 
    if ( keysHeld[SDLK_LEFT] ) {
        x -= 2;
        sy = 1;
        delay--;
    } else 
    if ( keysHeld[SDLK_RIGHT] ) {
        x += 2;
        sy = 3;
        delay--;
    } else {
        delay = 40;
        sx = 0;
    }
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


    running = 1, x = 100, y = 100, sx = 0, sy = 3;

    bitmap = SDL_LoadBMP("Chris.bmp");
    SDL_SetColorKey( bitmap, SDL_SRCCOLORKEY, SDL_MapRGB(bitmap->format, 255, 0, 255) ); 
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
