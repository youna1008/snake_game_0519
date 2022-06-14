#ifndef _GAMEHANDLER_H_
#define _GAMEHANDLER_H_

#include <iostream>
#include <curses.h>
#include <unistd.h>
#include <clocale>
#include <random>
#include <time.h>

#include "point.h"
#include "map.h"
#include "snake.h"
#include "item.h"

using namespace std;
#define SCREEN_SIZE 50

void alarm_handler(int sig);
bool moveNtest(WINDOW* board, SNAKE& snake, MAP& map, ITEM* item, int& key);
void playing_game();
void setting_with_playing();

#endif