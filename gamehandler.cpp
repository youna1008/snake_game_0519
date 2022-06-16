#include "gamehandler.h"

void alarm_handler(int sig)
{
  stop = 1;
}

void window_init(WINDOW* w, int size, const char* str)
{
  wclear(w);
  wborder(w, '|', '|', '-', '-', '+', '+', '+', '+');
  mvwprintw(w, 0, size, "%s", str);
}

void fail_print(MAP& map, int map_idx, WINDOW* board)
{
  wattron(board, COLOR_PAIR(3));
  map.MAP_init(map_idx);
  map.MAP_print(board);
  mvwprintw(board, (map.MAP_getMapSize() >> 1) + 1, (map.MAP_getMapSize() >> 1) - 2, "FAILED");
  wattroff(board, COLOR_PAIR(3));
  wrefresh(board);
}

bool mission_check(WINDOW* board, int size, int* point, int range)
{
  bool check_flag = true;
  
  window_init(board, size, "MISSION");

  if(point[0] >= 7)
  {
    mvwprintw(board, 2, 2, "Snake : clear");
  }
  else
  {
    mvwprintw(board, 2, 2, "Snake : Not clear");
    check_flag = false;
  }

  if (point[1] >= 3)
  {
    mvwprintw(board, 3, 2, "Growth : clear");
  }
  else
  {
    mvwprintw(board, 3, 2, "Growth : Not clear");
    check_flag = false;
  }

  if (point[2] >= 3)
  {
    mvwprintw(board, 4, 2, "Poison : clear");
  }
  else
  {
    mvwprintw(board, 4, 2, "Poison : Not clear");
    check_flag = false;
  }

  if (point[3] >= 3)
  {
    mvwprintw(board, 5, 2, "Gate : clear");
  }
  else
  {
    mvwprintw(board, 5, 2, "Gate : Not clear");
    check_flag = false;
  }

  return check_flag;
}

void mission_success_print(MAP& map, int map_idx, WINDOW* board)
{
  wattron(board, COLOR_PAIR(4));
  map.MAP_init(map_idx);
  map.MAP_print(board);
  mvwprintw(board, (map.MAP_getMapSize() >> 1) + 1, (map.MAP_getMapSize() >> 1) - 7, "MISSION COMPLETE!");
  wattroff(board, COLOR_PAIR(4));
  wrefresh(board);
}

void score_check(WINDOW* board, int size, int* point)
{
  window_init(board, size, "SCORE");
  mvwprintw(board, 2, 2, "Snake : (%d / 7)", point[0]);
  mvwprintw(board, 3, 2, "Growth : (%d / 3)", point[1]);
  mvwprintw(board, 4, 2, "Poison : (%d / 3)", point[2]);
  mvwprintw(board, 5, 2, "Gate : (%d / 3)", point[3]);
}

void count_print(MAP& map, int map_idx, WINDOW* board)
{
  wattron(board, COLOR_PAIR(4));
  map.MAP_init(map_idx);
  map.MAP_print(board);
  for(int i = 3; i > 0; i--)
  {
    mvwprintw(board, (map.MAP_getMapSize() >> 1) + 1, (map.MAP_getMapSize() >> 1) + 1, "%d", i);
    wrefresh(board);
    sleep(1);
  }

  mvwprintw(board, (map.MAP_getMapSize() >> 1) + 1, (map.MAP_getMapSize() >> 1) - 2, "START!");
  wattroff(board, COLOR_PAIR(4));
  wrefresh(board);
  sleep(1);
}

void game_clear_print(MAP& map, int map_idx, WINDOW* board)
{
  wattron(board, COLOR_PAIR(4));
  map.MAP_init(map_idx);
  map.MAP_print(board);
  mvwprintw(board, (map.MAP_getMapSize() >> 1) + 1, (map.MAP_getMapSize() >> 1) - 6, "GAME COMPLETE!!");
  wattroff(board, COLOR_PAIR(4));
  wrefresh(board);
}

bool moveNtest(WINDOW* board, MAP& map, SNAKE& snake, ITEM* item, GATE& gate, int& key, int* point)
{
  snake.SNAKE_move(key);
  
  POINT Shead = snake.SNAKE_getNthPoint(0);
  
  int map_val = map.MAP_getValue(Shead);

  switch (map_val)
  {
    case Growth :
      point[1]++;
      snake.SNAKE_EatItem(map, item);
      break;
    case Poison :
      point[2]++;
      snake.SNAKE_EatItem(map, item);
      break;
    
    case Gate :
      point[3]++;
      snake.SNAKE_EnterGate(map, gate, key);
      break;
  }

  point[0] = snake.SNAKE_getSnakeSize();

  bool test = snake.SNAKE_failure(map);

  for(int i = 0; i < 3; i++)
  {
    if(item[i].ITEM_getlifetime() >= 0)
      item[i].ITEM_insertMAP(map);
  }
  
  if(gate.GATE_getlifetime() >= 0)
    gate.GATE_insertMAP(map);
  snake.SNAKE_insertMAP(map);
  return test;
}

void playing_game(WINDOW* board[]) // Playing_Game으로 함수 변경하고 내부 함수 분리 및 아이템 추가.
{
  MAP map(MAP_SIZE);
  SNAKE snake;
  ITEM item[3];
  GATE g;
  snake.SNAKE_init(map);
  map.MAP_print(board[0]);
  
  wrefresh(board[0]);
  wrefresh(board[1]);
  wrefresh(board[2]);

  int key = int(KEY_LEFT);
  int temp;
  int fail_test = false;
  int mission_test = false;
  int mission_point[4] = {0, }; // snake size, growth point, poison point, gate point
  int index = 0;
  stop = 0;

  signal(SIGALRM, alarm_handler);
  timeout(0.5);

  while(index < 4)
  {
    while(!fail_test)
    {
      stop = 0;
      ualarm(500000, 0);
      int gate_temp = rand() % 10;
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
      if((gate_temp == 1) && (g.GATE_getlifetime() == -1))
      {
        g.GATE_create(map);
      }

      while(!stop)
      {
        temp = getch();
        if(temp >= int(KEY_DOWN) && temp <= int(KEY_RIGHT))
        {
          key = temp;
        }
      }
      
      fail_test = moveNtest(board[0], map, snake, item, g, key, mission_point);
      mission_test = mission_check(board[1], ((map.MAP_getMapSize() - 1) >> 1) - 3, mission_point, 4);

      for(int i = 0; i < 3; i++)
      {
        if(item[i].ITEM_getlifetime() >= 0)
          item[i].ITEM_clock();
        
        if(item[i].ITEM_getlifetime() >= 20)
          item[i].ITEM_delete(map);
      }
      
      if(g.GATE_getlifetime() >= 0)
      {
        g.GATE_clock();
      }

      if(g.GATE_getflag() >= 1)
      {
        g.GATE_flaginc();
      }

      if(((g.GATE_getlifetime() > 19) && (g.GATE_getflag() == 0)) || (g.GATE_getflag() >= snake.SNAKE_getSnakeSize() + 1))
      {
        g.GATE_delete(map);
      }

      map.MAP_print(board[0]);
      score_check(board[2], ((map.MAP_getMapSize() - 1) >> 1) - 2, mission_point);
      
      wrefresh(board[0]);
      wrefresh(board[1]);
      wrefresh(board[2]);

      if(mission_test == true)
      {
        mission_test = false;
        fail_test = false;
        key = int(KEY_LEFT);

        for(int i = 0; i < 4; i++)
        {
          mission_point[i] = 0;
        }

        item[0].ITEM_delete(map);
        item[1].ITEM_delete(map);
        item[2].ITEM_delete(map);

        g.GATE_delete(map);

        if(index < 3)
        {
          mission_success_print(map, 0, board[0]);
          sleep(1);
          count_print(map, 0, board[0]);
          map.MAP_init(index + 1); // change_map(map);
          snake.SNAKE_init(map);
          map.MAP_print(board[0]);
          index++;
        }

        else if(index == 3)
        {
          game_clear_print(map, 0, board[0]);
          sleep(1);
          map.MAP_print(board[0]);
          index++;
          break;
        }

        else
        {
          continue;
        }
      }
    }

    timeout(-1);
    if(fail_test == true)
    {
      fail_print(map, 0, board[0]);
      sleep(3);
      break;
    }
  }
}

void set_and_play()
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
  init_pair(4, COLOR_WHITE, COLOR_GREEN);

  border('|', '|', '-', '-', '+', '+', '+', '+');
  mvprintw(0, (SCREEN_SIZE >> 1) - 5, "SNAKE GAME");
  refresh();

  screen_board = newwin(SCREEN_SIZE, SCREEN_SIZE, 0, 0);
  wborder(screen_board, '|', '|', '-', '-', '+', '+', '+', '+');
  wbkgd(screen_board, COLOR_PAIR(2));
  wattron(screen_board, COLOR_PAIR(2));
  wrefresh(screen_board);
  
  game_board = subwin(screen_board, MAP_SIZE + 2, MAP_SIZE + 2, 1, 1);
  wbkgd(game_board, COLOR_PAIR(1));
  wattron(game_board, COLOR_PAIR(1));
  window_init(game_board, (MAP_SIZE >> 1) - 1, "MAP");
  
  mission_board = subwin(screen_board, 10, 21, 1, MAP_SIZE + 5);
  wbkgd(mission_board, COLOR_PAIR(1));
  wattron(mission_board, COLOR_PAIR(1));
  window_init(mission_board, ((MAP_SIZE - 1) >> 1) - 3, "MISSION");
  
  score_board = subwin(screen_board, 10, 21, 12, MAP_SIZE + 5);
  wbkgd(score_board, COLOR_PAIR(1));
  wattron(score_board, COLOR_PAIR(1));
  window_init(score_board, ((MAP_SIZE- 1) >> 1) - 2, "SCORE");
  
  

  WINDOW* board[3] = {game_board, mission_board, score_board};

  mvwprintw(board[1], 2, 2, "Snake : Not clear");
  mvwprintw(board[1], 3, 2, "Growth : Not clear");
  mvwprintw(board[1], 4, 2, "Poison : Not clear");
  mvwprintw(board[1], 5, 2, "Gate : Not clear");

  mvwprintw(board[2], 2, 2, "Snake : (%d / 7)", 3);
  mvwprintw(board[2], 3, 2, "Growth : (%d / 3)", 0);
  mvwprintw(board[2], 4, 2, "Poison : (%d / 3)", 0);
  mvwprintw(board[2], 5, 2, "Gate : (%d / 3)", 0);

  bool continue_flag = true;

  while(continue_flag)
  {
    playing_game(board);
    window_init(board[0], (MAP_SIZE >> 1) - 1, "MAP");
    mvwprintw(board[0], (MAP_SIZE >> 1) - 1, (MAP_SIZE >> 1) - 3, "%s", "CONTINUE?");
    mvwprintw(board[0], (MAP_SIZE >> 1), (MAP_SIZE >> 1) - 6, "%s", "(SELECT : ENTER)");
    mvwprintw(board[0], (MAP_SIZE >> 1) + 1, (MAP_SIZE >> 1) - 3, "%s", "> YES");
    mvwprintw(board[0], (MAP_SIZE >> 1) + 2, (MAP_SIZE >> 1) - 3, "%s", "  NO");
    wrefresh(board[0]);

    while(1)
    {
      int key_in = getch();
      int answer = !(int(key_in) % 2);
      if((key_in == int(KEY_UP)) || (key_in == int(KEY_DOWN)))
      {
        continue_flag = !answer;
        mvwprintw(board[0], (MAP_SIZE >> 1) + (1 + answer) % 2 + 1, (MAP_SIZE >> 1) - 3, "%s", " ");
        mvwprintw(board[0], (MAP_SIZE >> 1) + (2 + answer) % 2 + 1, (MAP_SIZE >> 1) - 3, "%s", ">");
        wrefresh(board[0]);
      }
      if(key_in == 10) // 10 : KEY enter
      {
        break;
      }
    }
  }
  
  delwin(game_board);
  delwin(score_board);
  delwin(mission_board);
  delwin(screen_board);
  endwin();
}