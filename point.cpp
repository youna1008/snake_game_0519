#include "point.h"

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
