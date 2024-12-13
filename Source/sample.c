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
	 .gamestate = GAMESTATE_INTRO,
	 .map = {{FLOOR}}
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
	init_map_walls(game.map);
	// Draw whole map
	draw_map(game);

	// LCD_DrawLine(0, 0, 200, 200, White);
	// init_timer(0, 0x1312D0 ); 						/* 50ms * 25MHz = 1.25*10^6 = 0x1312D0 */
	// init_timer(0, 0x6108 ); 						  /* 1ms * 25MHz = 25*10^3 = 0x6108 */
	// init_timer(0, 0x4E2 ); 						    /* 500us * 25MHz = 1.25*10^3 = 0x4E2 */
	init_timer(0, 0xC8); /* 8us * 25MHz = 200 ~= 0xC8 */

	enable_timer(0);

	LPC_SC->PCON |= 0x1; /* power-down	mode */
	LPC_SC->PCON &= ~(0x2);

	while (1)
	{
		__ASM("wfi");
	}
}