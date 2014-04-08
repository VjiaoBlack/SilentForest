#include "mapio.h"
#include "font.h"
#include "game.h"





void put_special(int grx, int gry, std::string tag, std::string content) {
    std::string tagc;
    std::string contentc;

    char* tagcp = (char*) malloc(sizeof(char) * (tag.size()+1));
    char* contentcp = (char*) malloc(sizeof(char) * (content.size() +1));

    strcpy(tagcp,tag.c_str());
    strcpy(contentcp,content.c_str());

    tagc.assign(tagcp);
    contentc.assign(contentcp);

    tiles(grx,gry).specials.insert(std::pair<std::string,std::string>(tagc,contentc));

}


void edit_specials(int grx, int gry) {
    char* buffer = (char*) malloc(sizeof(char) * 32);
    sprintf(buffer, "Editing Tile at %d, %d.", grx, gry);
    drawText(buffer, 200, 650);

    char* idbuf = (char*) malloc(sizeof(char) * 8);
    int x = 600, y = 650;

    // print out all the specials here
    if (tiles(grx,gry).specials.size() > 0) {
        for (std::map<std::string,std::string>::iterator it=tiles(grx,gry).specials.begin(); it != tiles(grx,gry).specials.end(); ++it) {
            sprintf(idbuf, "%s>%s", it->first.c_str(), it->second.c_str());
            y+= 32;
            if (y > 770) {
                x += 80;
            }
        }

    }
}

std::string data_to_string() {
    char* tostring = (char*) malloc(sizeof(char) * 64);
    sprintf(tostring,"h:%d w:%d b:{id:%d}\n", height, width, base_tile_id); 

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
    output.append("]\n");
    output.append("specials[\n");
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            if (tiles(x,y).specials.size() > 0) {
                output.append(parse_special(tiles(x,y)));
                output.append("\n");
            }
        }
    }
    output.append("]\nend");
    return output;
}

void edit_objects(int grx, int gry) {
    char* buffer = (char*) malloc(sizeof(char) * 32);
    sprintf(buffer, "Editing Tile at %d, %d.", grx, gry);
    drawText(buffer, 200, 650);

    char* idbuf = (char*) malloc(sizeof(char) * 8);
    int x = 570, y = 650;
    // print out all the objects here
    if (tiles(grx,gry).objects.size() > 0) {
        for (std::list<Object>::iterator it=tiles(grx,gry).objects.begin(); it != tiles(grx,gry).objects.end(); ++it) {
            sprintf(idbuf, "%d", it->id);
            y+= 32;
            if (y > 770) {
                x += 80;
            }
        }

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

std::string parse_special(Tile tile) {
    std::string output;
    char* special = (char*) malloc(sizeof(char) * 16);
    char* tostring = (char*) malloc(sizeof(char) * 64);
    sprintf(tostring,"%d;%d:{", tile.gridx, tile.gridy);
    output.append(tostring);
    std::map<std::string,std::string>::iterator it = tile.specials.begin();
    while (it != tile.specials.end()) {
        sprintf(special,"%s:%s", it->first.c_str(), it->second.c_str());
        output.append(special);
        if (++it != tile.specials.end()) {
            output.append(",");
        }
    }
    output.append("}");
    free(tostring);
    free(special);
    return output;
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


FILE* parse_input(int argc, char* argv[]) {
    int which = -1; // -1 is uninitialized, 0 is tiles, 1 is items, 2 is special
    int end = 0;
    int x, y, id;
    int i = 0;
    char test;
    FILE* mapfile;

    char cmd[32];
    if (argc == 2) {
        if ( (mapfile = fopen(argv[1], "r")) ) {
            end = feof(mapfile);
            height = 10;
            width = 15;
            fscanf(mapfile, "h:%d w:%d b:{id:%d}\n", &height, &width, &base_tile_id);
            // printf("scanned: height is %d, width is %d, base tile id is %d\n", height, width, base_tile_id);
            

            init_map(base_tile_id = 1);

            // printf("initialized map with given constraints.\n");
            while (!end) { // reads line by line ish
                end = feof(mapfile);
                switch (which) {
                    case -1: // currently seeing what to scan into
                        fgets(cmd, 16, mapfile);
                        if (*cmd != ']')
                            // printf("scanning for next command...\n");
                        switch (*cmd) {
                            case 't':
                                which = 0;
                                // printf("next command is |tiles|.\n");
                                break;
                            case 'i':
                                which = 1;
                                // printf("next command is |items|.\n");
                                break;
                            case 's':
                                which = 2;
                                // printf("next command is |special|.\n");
                                break;
                            case 'e':
                                end = 1;
                                // printf("next command is |end|.\n");
                                break;
                        }
                        break;
                    case 0: // currently scanning into tiles.
                        if (fscanf(mapfile,"{pos:%d;%d|id:%d}\n", &x, &y, &id)) {
                            change_tile(x,y,id);
                        } else {
                            which = -1;
                            // printf("tile input syntax broken; tile input has ended.\n");
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
                            // printf("item input syntax broken; item input has ended.\n");
                        }
                        break;
                    case 2: // the special
                        if (fscanf(mapfile,"%d;%d{", &x, &y)) {
                            // printf("found tile %d, %d to have special\n", x, y);
                            test = 'a';
                            int a = 1; // simple toggle to fix reading in {} / cursor seeking error
                            std::string tag;
                            std::string content;
                            while(test != '}' && (test = fgetc(mapfile)) != '}' ) {

                                if (a == 1)
                                    fseek(mapfile, 1, SEEK_CUR);
                                if (a == 0)
                                    fseek(mapfile, -1, SEEK_CUR);
                                a = 0;
                                i = 0;
                                while ( (test = fgetc(mapfile)) != ':') {
                                    tag[i++] = test;
                                }
                                tag[i] = '\0';
                                i = 0;
                                while ( (test = fgetc(mapfile)) != ',' && test != '}') {
                                    content[i++] = test;
                                }
                                content[i] = '\0';
                                i = 0;
                                // printf("putting tag and content into the specials map...\n");

                                put_special(x,y,tag,content);
                    
                            }
                            
                            which = -1;
                            // printf("special input syntax broken; special input has ended.\n");
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
        return NULL;
    }
    return mapfile;
}

void init_map(int id) {
    int x, y;
    draw_id = 1;
    selectedx = -1;
    selectedy = -1;
    tiles.resize(height*width);
    for (y = 0; y < height; y++) {
        for (x = 0; x < width; x++) {
            std::vector<Graphic>* graphics = new std::vector<Graphic>();
            Graphic* test1 = new Graphic(16, 16, 0, 0, id);
            Graphic* test2 = new Graphic(16, 16, 16, 0, id);
            Graphic* test3 = new Graphic(16, 16, 0, 16, id);
            Graphic* test4 = new Graphic(16, 16, 16, 16, id);
            graphics->push_back(*test1);
            graphics->push_back(*test2);
            graphics->push_back(*test3);
            graphics->push_back(*test4);

            Tile insert(id,x,y,*graphics); // fml TODO
            tiles(x,y) = (insert); 
        }
    }
}

void save_map(char* argv) {
    std::string tofile;
    tofile = data_to_string();
    std::ofstream out(argv);
    out << tofile;
    out.close();
}
