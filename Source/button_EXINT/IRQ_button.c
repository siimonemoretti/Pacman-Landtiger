#include "button.h"
#include "LPC17xx.h"
#include "../Pacman/pacman.h"

extern game_t game;

void EINT0_IRQHandler (void)
{		
	// Toggle gamestate
	if (game.gamestate == GAMESTATE_INTRO)
		game.gamestate = GAMESTATE_GAME;
	else if (game.gamestate == GAMESTATE_GAME)
		game.gamestate = GAMESTATE_INTRO;
	LPC_SC->EXTINT &= (1 << 0);     /* clear pending interrupt         */
}