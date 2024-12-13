#ifndef _DISPLAY_H_
#define _DISPLAY_H_

#include "../GLCD/GLCD.h"
#include "../Pacman/pacman.h"

extern pacman_t pacman;

void pause_screen();
void draw_map();
void move_pacman(uint16_t x, uint16_t y);
void init_map_walls(cell_t map[MAP_X][MAP_Y]);

#endif