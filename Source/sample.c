#include "LPC17xx.h"
#ifndef SIMULATOR
#include "adc/adc.h"
#include "Can/CAN.h"
#endif
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
	 .map = {
		  ROW_0, ROW_1, ROW_2, ROW_3, ROW_4, ROW_5, ROW_6, ROW_7, ROW_8, ROW_9, ROW_10, ROW_11, ROW_12, ROW_13,
		  ROW_13, ROW_12, ROW_11, ROW_10, ROW_9, ROW_8, ROW_7, ROW_6, ROW_5, ROW_4, ROW_3, ROW_2, ROW_1, ROW_0
	 },
	 .stats.score = 0,
	 .stats.lifes = 1,
	 .stats.time_left = 60,
	 .pill_counter = 240,
	 .ghost.x = 16,
	 .ghost.y = 13,
	 .ghost.mode = CHASE_MODE,
	 .ghost.respawn_time = 0
};

uint8_t button_pressed;
can_msg_t can_message = {			/* Game updates here, then when CAN2 receives value overwrites them to game_t */
	.time_left = 60,
	.lifes = 1, 
	.score = 0
};

int main(void)
{
	SystemInit();
	LCD_Initialization();
	BUTTON_init();
	JOYSTICK_init();
	#ifndef SIMULATOR /* In simulation we don't use CAN peripheral to update game's statistics */
	CAN_init();
	#endif
	LPC_SC->PCONP       |=  (1<<22);      /* Enable power to TIM2 block          */
  LPC_SC->PCONP       |=  (1<<23);      /* Enable power to TIM3 block          */
	LPC_PINCON->PINSEL1 |=  (1<<21);			/* Setting P0.26 out to speaker        */
	LPC_PINCON->PINSEL1 &= ~(1<<20);
	LPC_GPIO0->FIODIR |= (1<<26);					/* Setting P0.26 direction's     			 */
	
	// Place Pacman & Draw whole map
	game.map[game.pacman.x][game.pacman.y] = PACMAN;
	draw_map(game);
	// Compute random standard pills spawn time
	init_random_pow_pills();
	// Init timers (f = 10 Hz using board)
	#ifndef SIMULATOR
	init_timer(0,25000000/10);
	init_timer(1,25000000);
	init_timer(3,25000000/10);		/* Timer for ADC */
	init_RIT(0x007270E0);	
	#else
	init_timer(0,25000000/150);
	init_timer(1,25000000/30);	/* In simulation we don't wait for an entire second */
	init_RIT(0x000270E0);
	#endif
	enable_timer(0);
	enable_timer(1);
	enable_RIT();
	#ifndef SIMULATOR				/* With the simulator we don't play the music */
	enable_timer(3);
	ADC_init();
	#endif
	
	LPC_SC->PCON |= 0x1; /* power-down	mode */
	LPC_SC->PCON &= ~(0x2);

	while (1)
	{
		__ASM("wfi");
	}
}