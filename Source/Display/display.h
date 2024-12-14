#ifndef _DISPLAY_H_
#define _DISPLAY_H_

#include "../GLCD/GLCD.h"
#include "../Pacman/pacman.h"

#define W WALL
#define S STANDARD_PILL
#define P STANDARD_PILL
#define F FLOOR
#define T TELEPORT
// ----------- 1  2  3  4  5  6  7  8  9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28
#define ROW_0 {W, W, W, W, W, W, W, W, W, W, W, W, W, W, W, W, W, W, W, W, W, W, W, W, W, W, W, W}
#define ROW_1 {W, S, S, S, S, S, S, S, S, S, S, S, S, W, W, S, S, S, S, S, S, S, S, S, S, S, S, W}
#define ROW_2 {W, S, W, W, W, W, S, W, W, W, W, W, S, W, W, S, W, W, W, W, W, S, W, W, W, W, S, W}
#define ROW_3 {W, S, W, W, W, W, S, W, W, W, W, W, S, W, W, P, W, W, W, W, W, S, W, W, W, W, S, W}
#define ROW_4 {W, S, W, W, W, W, S, W, W, W, W, W, P, W, W, S, W, W, W, W, W, S, W, W, W, W, S, W}
#define ROW_5 {W, S, S, S, S, S, S, S, S, S, S, S, S, S, S, S, S, S, S, S, S, S, S, S, S, S, S, W}
#define ROW_6 {W, S, W, W, W, W, S, W, W, S, W, W, W, W, W, W, W, W, S, W, W, S, W, W, W, W, S, W}
#define ROW_7 {W, S, W, W, W, W, S, W, W, S, W, W, W, W, W, W, W, W, S, W, W, S, W, W, W, W, S, W}
#define ROW_8 {W, S, S, S, S, S, S, W, W, S, S, S, S, W, W, S, S, S, S, W, W, S, S, S, S, S, S, W}
#define ROW_9 {W, W, W, W, W, W, S, W, W, W, W, F, F, W, W, F, F, W, W, W, W, S, W, W, W, W, W, W}
#define ROW_10{F, F, F, F, F, W, S, W, W, W, W, F, F, W, W, F, F, W, W, W, W, S, W, F, F, F, F, F}
#define ROW_11{F, F, F, F, F, W, S, W, W, F, F, F, F, F, F, F, F, F, F, W, W, S, W, F, F, F, F, F}
#define ROW_12{W, W, W, W, W, W, S, W, W, F, F, F, F, W, W, F, F, F, F, W, W, S, W, W, W, W, W, W}
#define ROW_13{T, F, F, F, F, F, S, F, F, F, F, F, W, F, F, W, F, F, F, F, F, S, F, F, F, F, F, T}


extern game_t game;

void pause_screen();
void draw_map(game_t game);
void move_pacman(uint16_t x, uint16_t y);
void init_map_walls(cell_t map[MAP_X][MAP_Y]);
void draw_lifes();

#endif