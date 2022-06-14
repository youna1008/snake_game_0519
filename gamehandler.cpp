#include "gamehandler.h"

static volatile int stop = 0;

void alarm_handler(int sig)
{
  stop = 1;
}

bool moveNtest(WINDOW* board, SNAKE& snake, MAP& map, ITEM* item, int& key)
{
  snake.SNAKE_move(map, key);

  POINT Shead = snake.SNAKE_getNthPoint(0);
  
  int map_val = map.MAP_getValue(Shead);

  switch (map_val)
  {
    case Growth :
      snake.SNAKE_EatItem(map, item);
      break;
    case Poison :
      snake.SNAKE_EatItem(map, item);
      break;
  }

  bool test = snake.SNAKE_failure(map);

  for(int i = 0; i < 3; i++)
  {
    item[i].ITEM_insertMAP(map);
  }

  snake.SNAKE_insertMAP(map);
  return test;
}

void playing_game(WINDOW* board, MAP& map, SNAKE& snake)
{
  int key = int(KEY_LEFT);
  int temp;
  int fail_test = false;

  ITEM item[3];

  stop = 0;

  signal(SIGALRM, alarm_handler);
  timeout(0.5);

  while(!fail_test)
  {
    stop = 0;
    ualarm(500000, 0);

    int item_temp = rand() % 10;

    if(item_temp == 1)
    {
      for(int i = 0; i < 3; i++)
      {
        if (item[i].ITEM_getlifetime() == -1)
        {
          item[i].ITEM_create(map);
          break;
        }
      }
    }

    while(!stop)
    {
      temp = getch();
      if(temp >= int(KEY_DOWN) && temp <= int(KEY_RIGHT))
      {
        key = temp;
      }
    }
    
    fail_test = moveNtest(board, snake, map, item, key);

    for(int i = 0; i < 3; i++)
    {
      if(item[i].ITEM_getlifetime() >= 0)
        item[i].ITEM_clock();
      
      if(item[i].ITEM_getlifetime() >= 40)
        item[i].ITEM_delete(map);
    }

    if(fail_test == true)
  {
    map.MAP_init();
  }
    map.MAP_print(board);
    wrefresh(board);
  }

  timeout(-1);
}

void setting_with_playing()
{
  setlocale(LC_ALL, "");
  srand(time(NULL));

  WINDOW* screen_board;
  WINDOW* game_board;
  WINDOW* score_board;
  WINDOW* mission_board;

  initscr();
  keypad(stdscr, TRUE);
  noecho();
  curs_set(0);
  resize_term(SCREEN_SIZE, SCREEN_SIZE);
  start_color();
  init_pair(1, COLOR_WHITE, COLOR_BLACK);
  init_pair(2, COLOR_RED, COLOR_GREEN);
  init_pair(3, COLOR_WHITE, COLOR_RED);

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

  game_board = subwin(screen_board, start.MAP_getMapSize() + 2, start.MAP_getMapSize() + 2, 1, 1);
  wbkgd(game_board, COLOR_PAIR(1));
  wattron(game_board, COLOR_PAIR(1));
  wborder(game_board, '|', '|', '-', '-', '+', '+', '+', '+');
  mvwprintw(game_board, 0, (start.MAP_getMapSize() >> 1) - 1, "MAP");
  wrefresh(game_board);
  
  score_board = subwin(screen_board, 10, 21, 1, start.MAP_getMapSize() + 5);
  wbkgd(score_board, COLOR_PAIR(1));
  wattron(score_board, COLOR_PAIR(1));
  wborder(score_board, '|', '|', '-', '-', '+', '+', '+', '+');
  mvwprintw(score_board, 0, (20 >> 1) - 2, "SCORE");
  wrefresh(score_board);
  
  mission_board = subwin(screen_board, 10, 21, 12, start.MAP_getMapSize() + 5);
  wbkgd(mission_board, COLOR_PAIR(1));
  wattron(mission_board, COLOR_PAIR(1));
  wborder(mission_board, '|', '|', '-', '-', '+', '+', '+', '+');
  mvwprintw(mission_board, 0, (20 >> 1) - 3, "MISSION");
  wrefresh(mission_board);

  start.MAP_print(game_board);
  wrefresh(game_board);

  playing_game(game_board, start, snake);
  
  getch();
  delwin(game_board);
  delwin(score_board);
  delwin(mission_board);
  delwin(screen_board);
  endwin();
}