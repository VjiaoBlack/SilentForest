#include "game.h"
// Graphic class ################################

Graphic::Graphic(int h, int w, int sx, int sy, int id) {
    height = h;
    width = w;
    srcx = sx;
    srcy = sy; 
    height += 0 * id; // suppresses warnings
}

Graphic::Graphic() {
    height = 16;
    width = 16;
    srcx = 0;
    srcy = 0;
}

Graphic& Graphic::operator= (const Graphic *g) {
    height = g->height;
    width = g->width;
    srcx = g->srcx;
    srcy = g->srcy;
    return *this;

}

// Object class #################################

Object::Object(int identifier, Graphic* graphicpointer) {
    id = identifier;
    graphic = graphicpointer;
}

Object::Object(int identifier) {
    // find the correct graphic...
    id = identifier;
    graphic = NULL;
}

Object::Object() {
    id = 0;
    graphic = NULL;
}

// Tile class ###################################

Tile::Tile(int identifier, int gridxcor, int gridycor, std::vector<Graphic> graphicpointer) {
    id = identifier;
    gridx = gridxcor;
    gridy = gridycor;
    graphic = graphicpointer;
    objects.resize(0);
}

Tile::Tile(int ident, int grdx, int grdy) {
    id = ident;
    gridx = grdx;
    gridy = grdy;
    graphic.resize(4);
    objects.resize(0);
}

Tile::Tile() {
    id = 0;
    gridx = 0;
    gridy = 0;
    graphic.resize(4);
    objects.resize(0);
}

void Tile::additem(int id) {
    // printf("(placeholder) item of id:%d added to tile at %d,%d.\n", id, gridx, gridy);
    Object* object = new Object(id);
    objects.push_back(*object);
    return;
}

Tile& Tile::operator= (const Tile *t) {
        id = t->id;
        gridx = t->gridx;
        gridy = t->gridy;
        graphic = t->graphic;
        return *this;
    }
