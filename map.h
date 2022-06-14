#ifndef _MAP_H_
#define _MAP_H_
#include <iostream>
#include <curses.h>

#include "point.h"

using namespace std;

enum MAP_SETTING // 배열에 들어가는 숫자에 해당하는 값
{
  BackGround,
  Wall,
  Immune_Wall,
  Snake_Head,
  Snake_Body,
  Growth,
  Poison,
  Gate,
};

class MAP{
  int** m;
  int map_size;
  const char* image_set[8] = {" ", "X", " ", "H", "B", "G", "P", "@"};
  public:
  MAP(int size = 21);
  void MAP_init();
  void MAP_print(WINDOW* board) const;
  int MAP_getMapSize() const { return map_size; }
  int MAP_getValue(int x, int y) const { return m[x][y]; }
  int MAP_getValue(POINT& p) const { return m[p.POINT_getX()][p.POINT_getY()]; }
  void MAP_set(int x, int y, int value) { m[x][y] = value; }
  void MAP_set(POINT p, int value) { m[p.POINT_getX()][p.POINT_getY()] = value; }
};

#endif