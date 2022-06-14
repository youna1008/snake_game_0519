#include "point.h"

POINT POINT::POINT_rand(int map_size)
{
  POINT t;
  t.POINT_set(rand() % map_size, rand() % map_size);

  return t;
}

POINT& POINT::operator=(const POINT& p)
{
  x = p.x;
  y = p.y;

  return *this;
}

bool POINT::operator==(const POINT& p) const
{
  return ((x == p.x) && (y == p.y));
}

