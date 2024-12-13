#include <string.h>
#include "LPC17xx.h"
#include "timer.h"
#include "../GLCD/GLCD.h"
#include "../Pacman/pacman.h"
#include "../Display/display.h"

extern game_t game;

void TIMER0_IRQHandler(void)
{
	static gamestate_t prev_gs = GAMESTATE_GAME;
	if (game.gamestate == GAMESTATE_INTRO /* && game.gamestate != prev_gs*/ )
	{
		pause_screen();
		// TODO : Pause time too
	}
	else if (game.gamestate == GAMESTATE_GAME)
	{
		// If previous state overwrote central area, draw it back
		if (prev_gs == GAMESTATE_INTRO)
		{
			uint8_t x;
			for (x = 11; x < 17; x++)
			{
				LCD_DrawWall(x, 13, Blue);
				LCD_DrawWall(x, MAP_X - 14, Blue);
			}
		}
		// Read direction
		dir_t curr_dir = game.pacman.dir;
		// Check next position and if possible: move and update both pacman and map
		switch (curr_dir)
		{
		case UP:
			move_pacman(game.pacman.x - 1, game.pacman.y);
			break;
		case DOWN:
			move_pacman(game.pacman.x + 1, game.pacman.y);
			break;
		case RIGHT:
			move_pacman(game.pacman.x, game.pacman.y + 1);
			break;
		case LEFT:
			move_pacman(game.pacman.x, game.pacman.y - 1);
			break;
		default:
			break;
		}
	}
	prev_gs = game.gamestate;
	LPC_TIM0->IR = 1; /* clear interrupt flag */
	return;
}

void TIMER1_IRQHandler(void)
{
	LPC_TIM1->IR = 1; /* clear interrupt flag */
	return;
}
