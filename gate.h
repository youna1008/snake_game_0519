#ifndef _GATE_H_
#define _GATE_H_
#include <iostream>
#include <random>
#include "point.h"
#include "map.h"

class GATE{
  POINT GATE_PAIR[2];
  int lifetime;
  int gate_through;

  public:
  GATE()
  {
    lifetime = -1;
    gate_through = 0;
  }

  int GATE_getlifetime() const { return lifetime; }
  int GATE_getflag() const { return gate_through; }
  void GATE_flaginc() { gate_through += 1; }
  void GATE_create(MAP& map);
  void GATE_clock();
  void GATE_delete(MAP& map);
  void GATE_insertMAP(MAP& map);
  POINT GATE_returnPAIR(POINT& p);
};

#endif