#include <iostream>
#include <curses.h>
#include <clocale>
#include <unistd.h>
#include <signal.h>
#include <time.h>
#include <random>
using namespace std;
#define SCREEN_SIZE 50

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

static volatile int stop = 0;

void alarm_handler(int sig)
{
  stop = 1;
}

class POINT{
  int x;
  int y;
  public:
  POINT(int cor_x = 0, int cor_y = 0) { x = cor_x; y = cor_y; }
  void POINT_setX(int cor_x = 0) {x = cor_x; }
  void POINT_setY(int cor_y = 0) {y = cor_y; }
  void POINT_set(int cor_x = 0, int cor_y = 0)  { x = cor_x; y = cor_y; }
  int POINT_getX() { return x; }
  int POINT_getY() { return y; }

  //operators
  bool operator==(const POINT& b) const;
};

bool POINT::operator==(const POINT& b) const
{
  return ((x == b.x) && (y == b.y));
}

class MAP{
  public:
  int** m;
  int map_size;
  // wchar_t image_set[8] = {"⬛", "⬛", "⬛", "⬛", "⬛", "⬛", "⬛", "⬛"};
  const char* image_set[8] = {"\u2B1C", "\u2B1B", "\u2B1C", "\u2B1C", "\u2B1C", "\u2B1C", "\u2B1C", "\u2B1C"};
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
      // const char* temp = image_set[m[i][j]];
      // mvwprintw(board, 1 + i, 1 + j, "%c", image_set[m[i][j]]);
      // mvwprintw(board, 1 + i, 1 + j, image_set[m[i][j]]);
      // printf("%s", image_set[m[i][j]]);
      mvwprintw(board, 1 + i, 1 + j, "%d" , m[i][j]);
    }
    wmove(board, i + 1, 1);
  }
}



class SNAKE{
  int Snake_size;
  POINT s[10];
  public:
  SNAKE(int size = 3) : Snake_size(size) { }
  int SNAKE_getSnakeSize() { return Snake_size; };
  POINT SNAKE_getNthPoint(int idx) { return s[idx]; }
  void SNAKE_init(MAP& map);
  void SNAKE_move(MAP& map, int KEY_input);
  void SNAKE_insertMAP(MAP& map);
  bool SNAKE_failure(MAP& map);
};

void SNAKE::SNAKE_init(MAP& map)
{
  s[0].POINT_set(map.MAP_getMapSize() >> 1, (map.MAP_getMapSize() >> 1) - 1);
  s[1].POINT_set(map.MAP_getMapSize() >> 1, (map.MAP_getMapSize() >> 1));
  s[2].POINT_set(map.MAP_getMapSize() >> 1, (map.MAP_getMapSize() >> 1) + 1);
  map.MAP_set(s[0], Snake_Head);
  map.MAP_set(s[1], Snake_Body);
  map.MAP_set(s[2], Snake_Body);
}

void SNAKE::SNAKE_move(MAP& map, int KEY_input)
{
  for(int i = Snake_size - 1; i > 0; i--)
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

void SNAKE::SNAKE_insertMAP(MAP& map)
{
  map.MAP_init();
  map.MAP_set(s[0], Snake_Head);
  for(int i = 1; i < Snake_size; i++)
  {
    map.MAP_set(s[i], Snake_Body);
  }
}

bool SNAKE::SNAKE_failure(MAP& map) // 실패검사, 실패조건이면 true 반환, SNAKE_insertMAP 하기 전에 무조건 체크하고 반영하기.
{
  // 뱀 자체적인 실패
  if(Snake_size < 3)
    return true;

  for(int i = 1; i < Snake_size; i++)
  {
    if(s[0] == s[i])
      return true;
  }

  // 맵에 대한 실패
  if(map.MAP_getValue(s[0]) == Wall)
    return true;
  
  return false;
}

bool moveNtest(SNAKE& snake, MAP& map, WINDOW* board, int key)
{
  snake.SNAKE_move(map, key);
  bool test = snake.SNAKE_failure(map);
  snake.SNAKE_insertMAP(map);
  map.MAP_print(board);
  wrefresh(board);
  return test;
}

void kbin_loof(SNAKE& snake, MAP& map, WINDOW* board)
{
  int key = int(KEY_LEFT);
  int temp;
  int test = false;
  stop = 0;
  signal(SIGALRM, alarm_handler);
  while(!test)
  {
    stop = 0;
    alarm(1);
    while(!stop)
    {
      timeout(1);
      temp = getch();
      if(temp >= int(KEY_DOWN) && temp <= int(KEY_RIGHT))
      {
        key = temp;
      }
    }
    test = moveNtest(snake, map, board, key);
  }
}

int main()
{
  setlocale(LC_ALL, "");

  WINDOW* screen_board;
  WINDOW* game_board;
  WINDOW* score_board;
  WINDOW* mission_board;
  

  initscr();
  keypad(stdscr, TRUE);
  noecho();
  resize_term(SCREEN_SIZE, SCREEN_SIZE);
  start_color();
  init_pair(1, COLOR_WHITE, COLOR_BLACK);
  init_pair(2, COLOR_RED, COLOR_GREEN);

  border('|', '|', '-', '-', '+', '+', '+', '+');
  mvprintw(0, (SCREEN_SIZE >> 1) - 5, "SNAKE GAME");
  refresh();

  screen_board = newwin(SCREEN_SIZE, SCREEN_SIZE, 0, 0);
  wborder(screen_board, '|', '|', '-', '-', '+', '+', '+', '+');
  wbkgd(screen_board, COLOR_PAIR(2));
  wattron(screen_board, COLOR_PAIR(2));
  wrefresh(screen_board);

  MAP start;
  SNAKE snake;
  snake.SNAKE_init(start);
  game_board = subwin(screen_board, start.map_size + 2, start.map_size + 2, 1, 1);
  wbkgd(game_board, COLOR_PAIR(1));
  wattron(game_board, COLOR_PAIR(1));
  wborder(game_board, '|', '|', '-', '-', '+', '+', '+', '+');
  mvwprintw(game_board, 0, (start.map_size >> 1) - 1, "MAP");
  wrefresh(game_board);
  
  score_board = subwin(screen_board, 10, 21, 1, start.map_size + 5);
  wbkgd(score_board, COLOR_PAIR(1));
  wattron(score_board, COLOR_PAIR(1));
  wborder(score_board, '|', '|', '-', '-', '+', '+', '+', '+');
  mvwprintw(score_board, 0, (20 >> 1) - 2, "SCORE");
  wrefresh(score_board);
  
  mission_board = subwin(screen_board, 10, 21, 12, start.map_size + 5);
  wbkgd(mission_board, COLOR_PAIR(1));
  wattron(mission_board, COLOR_PAIR(1));
  wborder(mission_board, '|', '|', '-', '-', '+', '+', '+', '+');
  mvwprintw(mission_board, 0, (20 >> 1) - 3, "MISSION");
  wrefresh(mission_board);

  start.MAP_print(game_board);
  wrefresh(game_board);


  kbin_loof(snake, start, game_board);
  
  getch();
  delwin(game_board);
  delwin(score_board);
  delwin(mission_board);
  delwin(screen_board);
  endwin();
  return 0;
}