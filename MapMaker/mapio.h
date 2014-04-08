#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <list>
#include <vector>
#include <iostream>
#include <fstream>

#define tiles(x,y) tiles.at((y)*width + (x))

class Tile;
class Object;

extern std::vector<Tile> tiles;
extern int width;
extern int height;
extern int base_tile_id;
extern int draw_id;
extern int selectedx;
extern int selectedy;

std::string parse_tile(Tile*);
std::string data_to_string();
void edit_objects(int,int);
void edit_specials(int,int);
void put_special(int, int, std::string, std::string);
std::string parse_tile(Tile tile);
std::string parse_item(Tile tile);
std::string parse_special(Tile tile);

void change_tile(int,int,int);
void update_tile(int,int,int);
void init_map(int);
void save_map(char*);
FILE* parse_input(int argc, char* argv[]);
