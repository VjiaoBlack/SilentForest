#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <string>
#include <list>
#include <map>

// Graphic class ################################
class Graphic {
public: 
    int height, width;
    int srcx, srcy;

    Graphic& operator= (const Graphic *g);
    Graphic(int, int, int, int, int);
    Graphic();
};



// Object class #################################
class Object {
public:
    int id;
    Graphic* graphic;
    Object(int, Graphic*);
    Object(int);
    Object();
};

// Tile class ###################################
class Tile {
public:
    int id; // 0-999 walkable, 1000-1999 unwalkable, 2000-2999 special tile (launches scene), 3000-3999 special tile (interactive) ...
    int gridx, gridy;
    std::vector<Graphic> graphic;
    std::list<Object> objects;
    std::map<std::string,std::string> specials;

    Tile(int, int, int, std::vector<Graphic>);
    Tile(int,int,int);
    Tile();
    Tile& operator= (const Tile *t);

    void additem(int id);
    // note that an actual grid should contain a linked list of tile at every cor.
};

