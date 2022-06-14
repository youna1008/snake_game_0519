#include "gamehandler.h"

void playing_game()
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
  mvwprintw(mission_board, 0, (20 >> 1) - 3, "%s", "\u0444");
  wrefresh(mission_board);

  start.MAP_print(game_board);
  wrefresh(game_board);
  
  getch();
  delwin(game_board);
  delwin(score_board);
  delwin(mission_board);
  delwin(screen_board);
  endwin();
}