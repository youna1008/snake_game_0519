#include "map.h"

MAP::MAP(int size) : map_size(size) {
  m = new int*[map_size];
  for(int i = 0; i < map_size; i++){
    m[i] = new int[map_size];
  }
  MAP_init();
}

void MAP::MAP_init() {
  for(int i = 0; i < map_size; i++)
  {
    for(int j = 0; j < map_size; j++)
    {
      if(i == 0 || i == map_size - 1)
      {
        if(j == 0 || j == map_size - 1)
        {
          m[i][j] = Immune_Wall;
        }

        else
        {
          m[i][j] = Wall;
        }
      }

      else if (j == 0 || j == map_size - 1)
      {
        m[i][j] = Wall;
      }

      else
      {
        m[i][j] = BackGround;
      }
    }
  }
}

void MAP::MAP_print(WINDOW* board) const {
  wmove(board, 1, 1);
  for(int i = 0;  i < map_size; i++)
  {
    for(int j = 0; j < map_size; j++)
    {
      mvwprintw(board, 1 + i, 1 + j, "%s" , image_set[m[i][j]]);
    }
  }
}
