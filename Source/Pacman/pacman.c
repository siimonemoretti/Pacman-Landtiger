#include "pacman.h"
#include "LPC17xx.h"
#include "../a_star/a_star.h"
#include <stdlib.h>

void init_random_pow_pills()
{
	uint16_t i;
	srand(LPC_TIM0->TC);
	for (i = 0; i < N_POW_PILLS; i++)
	{
		// We have 6 power pills, and we generate them before 30 s elapse
		game.power_pills[i] = (rand() % 5) + (5 * i);
	}
}

static uint16_t manhattan_distance(int16_t x1, int16_t y1, int16_t x2, int16_t y2)
{
	return abs(x1 - x2) + abs(y1 - y2);
}

static int8_t is_valid_move(int16_t x, int16_t y)
{
	return (x >= 0 && x < MAP_X && y >= 0 && y < MAP_Y && game.map[x][y] != WALL);
}

static int dx[4] = {+1, 0, -1, 0};
static int dy[4] = {0, +1, 0, -1};

void move_ghost()
{
	if (game.ghost.mode == CHASE_MODE)
	{
		// Delete ghost
		draw_cell(game.ghost.x, game.ghost.y, game.map[game.ghost.x][game.ghost.y]);
		// Move towards pacman
		dir_t dir = a_star(game.map, game.ghost.x, game.ghost.y, game.pacman.x, game.pacman.y);
		if(dir != NUM_DIRS){
				game.ghost.x += dx[dir];
				game.ghost.y += dy[dir];
		}
		LCD_DrawGhost(game.ghost.x, game.ghost.y, Red);
	}
	else if (game.ghost.mode == FRIGHTENED_MODE)
	{
		// Move away from pacman, but slowed down
		if (game.stats.time_left % 2 == 0) // Slowing down Ghost by 50 percent
		{
			return;
		}
		// Delete ghost
		draw_cell(game.ghost.x, game.ghost.y, game.map[game.ghost.x][game.ghost.y]);
		int opposite_x = MAP_X - game.pacman.x - 1;
		int opposite_y = MAP_Y - game.pacman.y - 1;
		dir_t dir = a_star(game.map, game.ghost.x, game.ghost.y, opposite_x, opposite_y);
		if (is_valid_move(game.ghost.x + dx[dir], game.ghost.y + dy[dir]))
		{
			game.ghost.x += dx[dir];
			game.ghost.y += dy[dir];
		}
		LCD_DrawGhost(game.ghost.x, game.ghost.y, Blue);
	}
}