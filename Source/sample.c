#include "LPC17xx.h"
#include "RIT/RIT.h"
#include "GLCD/GLCD.h"
#include "timer/timer.h"
#include "Pacman/pacman.h"
#include "Display/display.h"
#include "Joystick/joystick.h"
#include "button_EXINT/button.h"

#ifdef SIMULATOR
extern uint8_t ScaleFlag; // <- ScaleFlag needs to visible in order for the emulator to find the symbol (can be placed also inside system_LPC17xx.h but since it is RO, it needs more work)
#endif

game_t game  = {
	 .pacman = {
		  .x = START_X,
		  .y = START_Y,
		  .dir = NUM_DIRS,
		  .color = Yellow,
	 },
	 .gamestate = GAMESTATE_GAME,
	 .map = {
		  ROW_0, ROW_1, ROW_2, ROW_3, ROW_4, ROW_5, ROW_6, ROW_7, ROW_8, ROW_9, ROW_10, ROW_11, ROW_12, ROW_13,
		  ROW_13, ROW_12, ROW_11, ROW_10, ROW_9, ROW_8, ROW_7, ROW_6, ROW_5, ROW_4, ROW_3, ROW_2, ROW_1, ROW_0
	 },
	 .score = 0,
	 .lifes = 1
};

int main(void)
{
	SystemInit();
	LCD_Initialization();
	BUTTON_init();
	JOYSTICK_init();
	init_RIT(0x004C4B40);
	enable_RIT();
	
	// Initialize map
	// init_map_walls(game.map);
	// Draw whole map
	draw_map(game);
	// Init timer (f = 10 Hz using board)
	#ifndef SIMULATOR
	init_timer(0,25000000/10); 
	#else
	init_timer(0,25000000/360);
	#endif
	enable_timer(0);

	LPC_SC->PCON |= 0x1; /* power-down	mode */
	LPC_SC->PCON &= ~(0x2);

	while (1)
	{
		__ASM("wfi");
	}
}