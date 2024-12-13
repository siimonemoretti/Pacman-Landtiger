#include "LPC17xx.h"
#include "RIT.h"
#include "../Pacman/pacman.h"

extern game_t game;

/* Exploiting RIT functionalities to implement polling on the joystick) */
void RIT_IRQHandler (void)
{
	static int volatile tmp = 0;
	if((LPC_GPIO1->FIOPIN & (1<<29)) == 0){	
		// UP
		game.pacman.dir = UP;
		tmp++;
	}
	else if ((LPC_GPIO1->FIOPIN & (1<<28)) == 0) {
		// RIGHT
		game.pacman.dir = RIGHT;
		tmp++;
	}
	else if ((LPC_GPIO1->FIOPIN & (1<<27)) == 0) {
		// LEFT
		game.pacman.dir = LEFT;
		tmp++;
	}
	else if ((LPC_GPIO1->FIOPIN & (1<<26)) == 0) {
		// DOWN
		game.pacman.dir = DOWN;
		tmp++;
	}
	
  LPC_RIT->RICTRL |= 0x1;	/* clear interrupt flag */
	LPC_RIT->RICOUNTER = 0;
  return;
}
