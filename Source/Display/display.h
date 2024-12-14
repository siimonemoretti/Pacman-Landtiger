#ifndef _DISPLAY_H_
#define _DISPLAY_H_

#include "../GLCD/GLCD.h"
#include "../Pacman/pacman.h"

extern game_t game;

void pause_screen();
void draw_map(game_t game);
void move_pacman(uint16_t x, uint16_t y);
void init_map_walls(cell_t map[MAP_X][MAP_Y]);
void draw_lifes();

#endif