#include "snake.h"

void SNAKE::SNAKE_init(MAP& map)
{
  for(int i = 0; i < 10; i++)
  {
    s[i].POINT_set(map.MAP_getMapSize() >> 1, (map.MAP_getMapSize() >> 1) - 1 + i);
  }
  SNAKE_insertMAP(map);
}

void SNAKE::SNAKE_move(MAP& map, int KEY_input)
{
  for(int i = 9; i > 0; i--)
    {
      s[i] = s[i - 1];
    }

  if(KEY_input == int(KEY_UP))
  {
    s[0].POINT_set(s[0].POINT_getX() - 1, s[0].POINT_getY());
  }
  if(KEY_input == int(KEY_DOWN))
  {
    s[0].POINT_set(s[0].POINT_getX() + 1, s[0].POINT_getY());
  }
  if(KEY_input == int(KEY_LEFT))
  {
    s[0].POINT_set(s[0].POINT_getX(), s[0].POINT_getY() - 1);
  }
  if(KEY_input == int(KEY_RIGHT))
  {
    s[0].POINT_set(s[0].POINT_getX(), s[0].POINT_getY() + 1);
  }
}

bool SNAKE::SNAKE_failure(MAP& map) // 실패검사, 실패조건이면 true 반환, SNAKE_insertMAP 하기 전에 무조건 체크하고 반영하기.
{
  // 뱀 자체적인 실패
  if(snake_size < 3)
    return true;

  for(int i = 1; i < snake_size; i++)
  {
    if(s[0] == s[i])
      return true;
  }

  // 맵에 대한 실패
  if(map.MAP_getValue(s[0]) == Wall)
    return true;
  
  return false;
}

void SNAKE::SNAKE_insertMAP(MAP& map)
{
  for(int i = 9; i > 0; i--)
  {
    if(i >= snake_size)
      map.MAP_set(s[i], BackGround);
    else
      map.MAP_set(s[i], Snake_Body);
  }
  map.MAP_set(s[0], Snake_Head);
}

void SNAKE::SNAKE_EatItem(MAP& map, ITEM* &item) // if item is growth, snake_size++. if item poison, snake_size--
{
  POINT temp = s[0];
  int val = map.MAP_getValue(temp);
  switch (val)
  {
    case Growth :
      snake_size += 1;
      break;
    case Poison :
      snake_size -= 1;
      break;
  }

  int i = 0;
  for(i = 0; i < 3; i++)
  {
    if(item[i].ITEM_getPoint() == s[0])
    {
      item[i].ITEM_delete(map);
      break;
    }
  }
}
