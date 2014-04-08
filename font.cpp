#include "font.h"



void init_font() {
    char counter;


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
}

void drawText(std::string inputstr, int x, int y) {
    char* input = (char*) malloc(sizeof(char) * inputstr.size());
    strcpy(input, inputstr.c_str());
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

