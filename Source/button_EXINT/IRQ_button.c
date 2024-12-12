#include "button.h"
#include "LPC17xx.h"
#include "../Pacman/pacman.h"

extern gamestate_t gamestate;

void EINT0_IRQHandler (void)
{		
	// Toggle gamestate
	if (gamestate == GAMESTATE_INTRO)
		gamestate = GAMESTATE_GAME;
	else if (gamestate == GAMESTATE_GAME)
		gamestate = GAMESTATE_INTRO;
	LPC_SC->EXTINT &= (1 << 0);     /* clear pending interrupt         */
}