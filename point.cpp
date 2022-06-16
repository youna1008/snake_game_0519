#include "point.h"

POINT operator+(const POINT& p1, const POINT& p2)
{
  return POINT(p1.x + p2.x, p1.y + p2.y);
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

POINT POINT::POINT_rand(int map_size)
{
  POINT t;
  t.POINT_set(rand() % map_size, rand() % map_size);

  return t;
}