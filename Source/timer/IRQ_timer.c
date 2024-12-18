#include <string.h>
#include <stdio.h>
#include "LPC17xx.h"
#include "timer.h"
#include "../GLCD/GLCD.h"
#include "../Pacman/pacman.h"
#include "../Display/display.h"
#include "../RIT/RIT.h"
#include "../Can/CAN.h"

extern game_t game;
extern can_msg_t can_message;

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
			LCD_DrawWall(13, 11, Blue);
			LCD_DrawWall(13, 16, Blue);
			LCD_DrawWall(14, 11, Blue);
			LCD_DrawWall(14, 16, Blue);
			for (x = 12; x < 16; x++)
			{
				LCD_DrawFloor(13, x);
				LCD_DrawFloor(14, x);
			}
			// Re-enable counting
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
		if (game.stats.score != prev_sc)
		{
			// Update score
			if (game.stats.score >= 1000)
			{
				game.stats.score -= 1000;
				can_message.score -= 1000;
				game.stats.lifes++;
				draw_lifes();
			}
			// Half is 120 pixels lons, score is 5*8 = 40 pixels, so write SCORE @ X = 80
			GUI_Text(120 - (6*8), 3, (uint8_t*)"SCORE", White, BK_COLOR);
			char str[5];  // 4 digits + null terminator
			sprintf(str, "%04u", game.stats.score);
			GUI_Text(120 - (5*8), 15, (uint8_t*)str, White, BK_COLOR);
			prev_sc = game.stats.score;
		}
	}
	/*Send CAN message*/
	CAN_TxMsg.data[0] = can_message.score;
	CAN_TxMsg.data[1] = can_message.score >> 8;
	CAN_TxMsg.data[2] = can_message.lifes;
	CAN_TxMsg.data[3] = can_message.time_left;
	CAN_TxMsg.len = 4;
	CAN_TxMsg.id = 2;
	CAN_TxMsg.format = STANDARD_FORMAT;
	CAN_TxMsg.type = DATA_FRAME;
	CAN_wrMsg (1, &CAN_TxMsg);
	prev_gs = game.gamestate;
	LPC_TIM0->IR = 1; /* clear interrupt flag */
	return;
}

void TIMER1_IRQHandler(void)
{
	static uint8_t index = 0, t = 0;
	// This is used for countdown
	draw_time_left();
	if(game.stats.time_left == 0)
	{
		// Lost
		disable_timer(0);
		disable_timer(1);
		disable_RIT();
		lost_screen();
	}	
	
	// Check if we have to spawn a power pill
	if(game.power_pills[index] == t)
	{
		// Exchange standard pill with random pill
		int r = rand() % 220;
		uint8_t x, y;
		for (x = 0; x < MAP_X; x++)
		{
			for (y = 0; y < MAP_Y; y++)
			{
				if (game.map[x][y] == STANDARD_PILL)
				{
					r--;
					if (r == 0)
					{
						game.map[x][y] = POWER_PILL;
						LCD_DrawPowerPill(x, y, Red);
					}
				}
			}
		}
		index++;
	}
	can_message.time_left--;
	t++;
	LPC_TIM1->IR = 1; /* clear interrupt flag */
	return;
}
