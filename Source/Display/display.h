#ifndef _DISPLAY_H_
#define _DISPLAY_H_

#include "../GLCD/GLCD.h"
#include "../Pacman/pacman.h"

extern pacman_t pacman;

void pause_screen();
void print_pacman();
void draw_map();

#endif