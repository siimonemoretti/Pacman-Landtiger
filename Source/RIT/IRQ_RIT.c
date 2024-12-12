#include "LPC17xx.h"
#include "RIT.h"
#include "../Pacman/pacman.h"

extern pacman_t pacman;

/* Exploiting RIT functionalities to implement polling on the joystick) */
void RIT_IRQHandler (void)
{
	static int tmp = 0;
	if((LPC_GPIO1->FIOPIN & (1<<29)) == 0){	
		// UP
		pacman.dir = UP;
	}
	else if ((LPC_GPIO1->FIOPIN & (1<<28)) == 0) {
		// RIGHT
		pacman.dir = RIGHT;
	}
	else if ((LPC_GPIO1->FIOPIN & (1<<27)) == 0) {
		// LEFT
		pacman.dir = LEFT;
	}
	else if ((LPC_GPIO1->FIOPIN & (1<<26)) == 0) {
		// DOWN
		pacman.dir = DOWN;
	}
	
  LPC_RIT->RICTRL |= 0x1;	/* clear interrupt flag */
	
  return;
}
