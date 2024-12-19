#include "pacman.h"
#include "LPC17xx.h"
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

void move_ghost()
{
	if (game.ghost.mode == CHASE_MODE)
	{
		// Initialize the best distance to the current distance
		int best_distance = 10000;
		// Search for the best available move between: up, down, left, right
		int i, k;
		int16_t new_x = game.ghost.x;
		int16_t new_y = game.ghost.y;
		// Direction offsets for up, down, left, and right
		int directions[4][2] = {
				{0, -1}, // Up
				{0, 1},	 // Down
				{-1, 0}, // Left
				{1, 0}	 // Right
		};

		// Iterate over each direction
		for (i = 0; i < 4; i++)
		{
			int new_x_candidate = game.ghost.x + directions[i][0];
			int new_y_candidate = game.ghost.y + directions[i][1];

			if (is_valid_move(new_x_candidate, new_y_candidate))
			{
				int distance = manhattan_distance(new_x_candidate, new_y_candidate, game.pacman.x, game.pacman.y);
				if (distance < best_distance)
				{
					best_distance = distance;
					new_x = new_x_candidate;
					new_y = new_y_candidate;
				}
			}
		}
		// Move the ghost's position
		game.ghost.x = new_x;
		game.ghost.y = new_y;
		LCD_DrawGhost(game.ghost.x, game.ghost.y, Red);
	}
	else if (game.ghost.mode == FRIGHTENED_MODE)
	{
		LCD_DrawGhost(game.ghost.x, game.ghost.y, Blue);
	}
}