#include <string.h>
#include "LPC17xx.h"
#include "timer.h"
#include "../GLCD/GLCD.h"
#include "../Pacman/pacman.h"
#include "../Display/display.h"

extern pacman_t pacman;
extern gamestate_t gamestate;
gamestate_t prev_gs = GAMESTATE_GAME;

void TIMER0_IRQHandler(void)
{

	if (gamestate == GAMESTATE_INTRO && gamestate != prev_gs)
	{
		pause_screen();
	}
	else if (gamestate == GAMESTATE_GAME)
	{
		// Read direction
		dir_t curr_dir = pacman.dir;
		// Check next position and if possible: move and update both pacman and map
		switch (curr_dir)
		{
		case UP:
			move_pacman(pacman.pos.x, pacman.pos.y + 1);
			break;
		case DOWN:
			move_pacman(pacman.pos.x, pacman.pos.y - 1);
			break;
		case RIGHT:
			move_pacman(pacman.pos.x + 1, pacman.pos.y);
			break;
		case LEFT:
			move_pacman(pacman.pos.x - 1, pacman.pos.y);
			break;
		default:
			break;
		}
	}
	prev_gs = gamestate;
	LPC_TIM0->IR = 1; /* clear interrupt flag */
	return;
}

void TIMER1_IRQHandler(void)
{
	LPC_TIM1->IR = 1; /* clear interrupt flag */
	return;
}
