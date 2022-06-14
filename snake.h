#ifndef _SNAKE_H_
#define _SNAKE_H_
#include <iostream>
#include <curses.h>

#include "point.h"
#include "map.h"

using namespace std;

class SNAKE{
  public:
  int snake_size;
  POINT s[10];
  
  SNAKE(int size = 3) : snake_size(size) { }
  int SNAKE_getSnakeSize() { return snake_size; };
  void SNAKE_setSnakeSize(int value) { snake_size = value; }
  POINT SNAKE_getNthPoint(int idx) { return s[idx]; }
  void SNAKE_init(MAP& map);
  void SNAKE_move(MAP& map, int KEY_input);
  void SNAKE_insertMAP(MAP& map);
  bool SNAKE_failure(MAP& map);
};

#endif