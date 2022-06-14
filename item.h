#ifndef _ITEM_H_
#define _ITEM_H_
#include <iostream>
#include <random>
#include "point.h"
#include "map.h"

class ITEM{
public:
    POINT PG;
    int PG_lifetime;//-1 init_pair
    int PG_type;
    ITEM(){
      // check not body xy and create xy
      PG = POINT(0, 0);
      PG_type = 0; // make item type
      PG_lifetime = -1;
    }

    void ITEM_create(MAP& map);
    int ITEM_getlifetime() { return PG_lifetime; }
    void ITEM_clock();
    void ITEM_delete(MAP& map);
    void ITEM_insertMAP(MAP& map);
    POINT ITEM_getPoint() { return PG; }
};

#endif