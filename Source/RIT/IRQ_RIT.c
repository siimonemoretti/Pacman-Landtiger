#include "LPC17xx.h"
#include "RIT.h"
#include "../Pacman/pacman.h"

extern game_t game;
extern uint8_t button_pressed;

/* Exploiting RIT functionalities to implement polling on the joystick */
void RIT_IRQHandler (void)
{
	if (button_pressed == 1)
	{
		if ( (LPC_GPIO2->FIOPIN & (1<<10)) == 0 )
		{
			// Toggle gamestate
			if (game.gamestate == GAMESTATE_INTRO)
				game.gamestate = GAMESTATE_GAME;
			else if (game.gamestate == GAMESTATE_GAME)
				game.gamestate = GAMESTATE_INTRO;
		}
		button_pressed++;
	}
	else
	{
			button_pressed=0;
			NVIC_EnableIRQ(EINT0_IRQn);
			LPC_PINCON->PINSEL4    |= (1 << 20);
	}
	
	if((LPC_GPIO1->FIOPIN & (1<<29)) == 0){	
		game.pacman.dir = UP;
	}
	else if ((LPC_GPIO1->FIOPIN & (1<<28)) == 0) {
		game.pacman.dir = RIGHT;
	}
	else if ((LPC_GPIO1->FIOPIN & (1<<27)) == 0) {
		game.pacman.dir = LEFT;
	}
	else if ((LPC_GPIO1->FIOPIN & (1<<26)) == 0) {
		game.pacman.dir = DOWN;
	}
	LPC_RIT->RICOUNTER = 0;
  LPC_RIT->RICTRL |= 0x1;	/* clear interrupt flag */
  return;
}
