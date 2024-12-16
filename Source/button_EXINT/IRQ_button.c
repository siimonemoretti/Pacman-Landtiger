#include "button.h"
#include "LPC17xx.h"
#include "../Pacman/pacman.h"

extern game_t game;
extern uint8_t button_pressed;

void EINT0_IRQHandler (void)
{
	// Handle debouncing
	button_pressed = 1;
	NVIC_DisableIRQ(EINT0_IRQn);
	LPC_PINCON->PINSEL4    &= ~(1 << 20);		// Set pin to GPIO
	
	LPC_SC->EXTINT &= (1 << 0);     /* clear pending interrupt         */
}