#include "item.h"

void ITEM::ITEM_create(MAP &map) 
{
  // check not body xy and create xy
  //int a,b;

  do{
    PG = POINT::POINT_rand(map.MAP_getMapSize());
  }while(map.MAP_getValue(PG) != 0);

  PG_lifetime = 0;
  PG_type = rand()% 2 + 5; // make item type

  map.MAP_set(PG, PG_type);
}

void ITEM::ITEM_clock() 
{ //while 1sec
      PG_lifetime++;
}

void ITEM::ITEM_delete(MAP& map) 
{
  PG_lifetime = -1;
  map.MAP_set(PG, BackGround);
}

void ITEM::ITEM_insertMAP(MAP& map)
{
  map.MAP_set(PG, PG_type);
}