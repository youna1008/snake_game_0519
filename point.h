#ifndef _POINT_H_
#define _POINT_H_
#include <iostream>

using namespace std;

class POINT{
  int x;
  int y;
  public:
  POINT(int coor_x = 0, int coor_y = 0) { x = coor_x; y = coor_y; }
  void POINT_setX(int coor_x = 0) {x = coor_x; }
  void POINT_setY(int coor_y = 0) {y = coor_y; }
  void POINT_set(int coor_x = 0, int coor_y = 0)  { x = coor_x; y = coor_y; }
  int POINT_getX() { return x; }
  int POINT_getY() { return y; }

  static POINT POINT_rand(int map_size);

  //operators
  POINT& operator=(const POINT& p);
  bool operator==(const POINT& p) const;
};

#endif