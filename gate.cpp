#include "gate.h"

void GATE::GATE_create(MAP& map)
{
  lifetime = 0;
  gate_through = 0;

  int GATE_idx = 0;

  while(GATE_idx < 2)
  {
    POINT temp;
    temp = POINT::POINT_rand(map.MAP_getMapSize());
    if(map.MAP_getValue(temp) == Wall)
    {
      map.MAP_set(temp, Gate);
      GATE_PAIR[GATE_idx] = temp;
      GATE_idx++;
    }
  }
}

void GATE::GATE_delete(MAP& map)
{
  lifetime = -1;
  gate_through = 0;
  map.MAP_set(GATE_PAIR[0], Wall);
  map.MAP_set(GATE_PAIR[1], Wall);
}

void GATE::GATE_clock()
{
  lifetime++;
}

POINT GATE::GATE_returnPAIR(POINT& p)
{
  if(GATE_PAIR[0] == p)
  {
    return GATE_PAIR[1];
  }
  else
  {
    return GATE_PAIR[0];
  }
}

void GATE::GATE_insertMAP(MAP& map)
{
  map.MAP_set(GATE_PAIR[0], Gate);
  map.MAP_set(GATE_PAIR[1], Gate);
}
