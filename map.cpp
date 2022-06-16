#include "map.h"

MAP::MAP(int size) : map_size(size) {
  m = new int*[map_size];
  for(int i = 0; i < map_size; i++){
    m[i] = new int[map_size];
  }
  MAP_init();
}

MAP::~MAP()
{
  for(int i = 0; i < map_size; i++)
  {
    delete[] m[i];
  }

  delete[] m;

  m = nullptr;
}

void MAP::MAP_init(int index) {
  
    for(int i = 0; i < map_size; i++)
    {
      for(int j = 0; j < map_size; j++)
      {
        if(index == 1)
          m[i][j] = MAP_heart[i][j];

        else if(index == 2)
          m[i][j] = MAP_smile[i][j];

        else if(index == 3)
          m[i][j] = MAP_cross_heart[i][j];

        else
          m[i][j] = MAP_basic[i][j];
      }
    }
}

void MAP::MAP_print(WINDOW* board) const {
  for(int i = 0;  i < map_size; i++)
  {
    for(int j = 0; j < map_size; j++)
    {
      mvwprintw(board, 1 + i, 1 + j, "%s" , image_set[m[i][j]]);
    }
  }
}
