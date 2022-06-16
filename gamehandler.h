#ifndef _GAMEHANDLER_H_
#define _GAMEHANDLER_H_

#include <iostream>
#include <curses.h>
#include <unistd.h>
#include <signal.h>
#include <time.h>
#include <random>
#include <clocale>

#include "map.h"
#include "item.h"
#include "gate.h"
#include "snake.h"

using namespace std;
#define SCREEN_SIZE 50
#define MAP_SIZE 21

static volatile int stop = 0;

void alarm_handler(int sig);
void window_init(WINDOW* w, int size, const char* str);
void fail_print(MAP& map, int map_idx, WINDOW* board);
bool mission_check(WINDOW* board, int size, int* point, int range);
void mission_success_print(MAP& map, int map_idx, WINDOW* board);
void score_check(WINDOW* board, int size, int* point);
void count_print(MAP& map, int map_idx, WINDOW* board);
void game_clear_print(MAP& map, int map_idx, WINDOW* board);
bool moveNtest(WINDOW* board, MAP& map, SNAKE& snake, ITEM* item, GATE& gate, int& key, int* point);
void playing_game(WINDOW* board[]); // Playing_Game으로 함수 변경하고 내부 함수 분리 및 아이템 추가.
void set_and_play();

#endif