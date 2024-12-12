#include <string.h>
#include "LPC17xx.h"
#include "timer.h"
#include "../GLCD/GLCD.h"
#include "../Pacman/pacman.h"
#include "../Display/display.h"

extern pacman_t pacman;
extern gamestate_t gamestate;
gamestate_t prev_gs;

void TIMER0_IRQHandler (void)
{
	if (gamestate != prev_gs)
	{
		if(gamestate == GAMESTATE_INTRO)
		{
			pause_screen();
		}
		else if (gamestate == GAMESTATE_GAME)
		{
			// Read direction
			dir_t curr_dir = pacman.dir;
			// Check next position and move
			switch(curr_dir)
			{
				case UP:
					if (pacman.pos.y + 1 < MAX_Y)
						pacman.pos.y++;
					break;
				case DOWN:
					if (pacman.pos.y > 1)
						pacman.pos.y--;
					break;
				case RIGHT:
					if (pacman.pos.x + 1 < MAX_X)
						pacman.pos.x++;
					break;
				case LEFT:
					if (pacman.pos.x > 1)
						pacman.pos.x--;
					break;
				default:
					break;
			}
			// Print pacman
			print_pacman();
		}
	}
	prev_gs = gamestate;
  LPC_TIM0->IR = 1;			/* clear interrupt flag */
  return;
}

void TIMER1_IRQHandler (void)
{
  LPC_TIM1->IR = 1;			/* clear interrupt flag */
  return;
}
