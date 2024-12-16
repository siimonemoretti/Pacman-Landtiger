#include <string.h>
#include <stdio.h>
#include "LPC17xx.h"
#include "timer.h"
#include "../GLCD/GLCD.h"
#include "../Pacman/pacman.h"
#include "../Display/display.h"
#include "../RIT/RIT.h"

extern game_t game;

void TIMER0_IRQHandler(void)
{
	static gamestate_t prev_gs = GAMESTATE_GAME;
	static uint16_t prev_sc = 1;
	if (game.gamestate == GAMESTATE_INTRO)
	{
		pause_screen();
		disable_timer(1);
	}
	else if (game.gamestate == GAMESTATE_GAME)
	{
		// If previous state overwrote central area, draw it back
		if (prev_gs == GAMESTATE_INTRO)
		{
			uint8_t x;
			for (x = 11; x < 17; x++)
			{
				LCD_DrawFloor(13, x);
				LCD_DrawFloor(MAP_X - 14, x);
			}
			enable_timer(1);
		}
		// Read direction
		dir_t curr_dir = game.pacman.dir;
		// Check next position and if possible: move and update both pacman and map
		switch (curr_dir)
		{
		case UP:
			move_pacman(game.pacman.x + 1, game.pacman.y);
			break;
		case DOWN:
			move_pacman(game.pacman.x - 1, game.pacman.y);
			break;
		case RIGHT:
			move_pacman(game.pacman.x, game.pacman.y + 1);
			break;
		case LEFT:
			move_pacman(game.pacman.x, game.pacman.y - 1);
			break;
		default:
			break;
		}
		// Check if won
		if (game.pill_counter == 0)
		{
			disable_timer(0);
			disable_timer(1);
			disable_RIT();
			win_screen();
		}
		// Print score
		if (game.score != prev_sc)
		{
			// Update score
			if (game.score >= 1000)
			{
				game.score -= 1000;
				game.lifes++;
				draw_lifes();
			}
			// Half is 120 pixels lons, score is 5*8 = 40 pixels, so write SCORE @ X = 80
			GUI_Text(120 - (6*8), 3, (uint8_t*)"SCORE", White, BK_COLOR);
			char str[5];  // 4 digits + null terminator
			sprintf(str, "%04u", game.score);
			GUI_Text(120 - (5*8), 15, (uint8_t*)str, White, BK_COLOR);
			prev_sc = game.score;
		}
	}
	prev_gs = game.gamestate;
	LPC_TIM0->IR = 1; /* clear interrupt flag */
	return;
}

void TIMER1_IRQHandler(void)
{
	// This is used for countdown
	draw_time_left();
	if(game.time_left-- == 0)
	{
		// Lost
		disable_timer(0);
		disable_timer(1);
		disable_RIT();
		lost_screen();
	}	
	
	LPC_TIM1->IR = 1; /* clear interrupt flag */
	return;
}
