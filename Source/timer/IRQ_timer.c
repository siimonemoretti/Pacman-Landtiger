#include <string.h>
#include <stdio.h>
#include "LPC17xx.h"
#include "timer.h"
#include "../GLCD/GLCD.h"
#include "../Pacman/pacman.h"
#include "../Display/display.h"
#include "../RIT/RIT.h"
#include "../Can/CAN.h"
#include "../adc/adc.h"
#include "../music/music.h"

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
		// Draw back original cell's type, then move ghost
		draw_cell(game.ghost.x, game.ghost.y, game.map[game.ghost.x][game.ghost.y]);
		move_ghost();
		// Check if ghost caught pacman
		if (game.ghost.x == game.pacman.x && game.ghost.y == game.pacman.y)
		{
			// Check ghost's mode
			if (game.ghost.mode == CHASE_MODE)
			{
				// Decrease remaining lifes
				game.stats.lifes--;
				if (game.stats.lifes <= 0)
				{
					disable_timer(0);
					disable_timer(1);
					disable_timer(2);
					disable_timer(3);
					disable_RIT();
					lost_screen();
				}
				else
				{
					// Respawn pacman
					game.pacman.x = 13;
					game.pacman.y = 13;
					game.pacman.dir = RIGHT;
					LCD_DrawPacman(13, 13, RIGHT, Yellow);
					draw_lifes();
				}
			}
			else if (game.ghost.mode == FRIGHTENED_MODE)
			{
				// Increase score
				game.stats.score += 100;
				can_message.score += 100;
				// TODO : Respawn ghost after 3 seconds
			}
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
			GUI_Text(120 - (6 * 8), 3, (uint8_t *)"SCORE", White, BK_COLOR);
			char str[5]; // 4 digits + null terminator
			sprintf(str, "%04u", game.stats.score);
			GUI_Text(120 - (5 * 8), 15, (uint8_t *)str, White, BK_COLOR);
			prev_sc = game.stats.score;
		}
	}
#ifndef SIMULATOR
	/*Send CAN message*/
	CAN_TxMsg.data[0] = can_message.score;
	CAN_TxMsg.data[1] = can_message.score >> 8;
	CAN_TxMsg.data[2] = can_message.lifes;
	CAN_TxMsg.data[3] = can_message.time_left;
	CAN_TxMsg.len = 4;
	CAN_TxMsg.id = 2;
	CAN_TxMsg.format = STANDARD_FORMAT;
	CAN_TxMsg.type = DATA_FRAME;
	CAN_wrMsg(1, &CAN_TxMsg);
#else
	game.stats.score = can_message.score;
	game.stats.lifes = can_message.lifes;
	game.stats.time_left = can_message.time_left;
#endif
	prev_gs = game.gamestate;
	LPC_TIM0->IR = 1; /* clear interrupt flag */
	return;
}

void TIMER1_IRQHandler(void)
{
	static uint8_t index = 0, t = 0;
	// This is used for countdown
	draw_time_left();
	if (game.stats.time_left == 0)
	{
		// Lost
		disable_timer(0);
		disable_timer(1);
		disable_RIT();
		lost_screen();
		disable_timer(2);
		disable_timer(3);
	}

	// Check Ghost's mode
	if (game.ghost.frightened_cnt > 0)
	{
		game.ghost.frightened_cnt--;
		game.ghost.mode = FRIGHTENED_MODE;
	}
	else
	{
		game.ghost.mode = CHASE_MODE;
	}

	// Check if we have to spawn a power pill
	if (game.power_pills[index] == t)
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

uint16_t SinTable[45] =
	 {
		  410, 467, 523, 576, 627, 673, 714, 749, 778,
		  799, 813, 819, 817, 807, 789, 764, 732, 694,
		  650, 602, 550, 495, 438, 381, 324, 270, 217,
		  169, 125, 87, 55, 30, 12, 2, 0, 6,
		  20, 41, 70, 105, 146, 193, 243, 297, 353};

void TIMER2_IRQHandler(void)
{
	static int sineticks = 0;
	/* DAC management */
	static int currentValue;
	currentValue = SinTable[sineticks];
	currentValue -= 410;
	currentValue /= 1;
	currentValue += 410;
	LPC_DAC->DACR = currentValue << 6;
	sineticks++;
	if (sineticks == 45)
		sineticks = 0;

	LPC_TIM2->IR = 1; /* clear interrupt flag */
	return;
}

#define RIT_SEMIMINIMA 8
#define RIT_MINIMA 16
#define RIT_INTERA 32

#define UPTICKS 1

// SHORTENING UNDERTALE: TOO MANY REPETITIONS
NOTE song[] =
	 {
		  // Jingle Bells, Jingle Bells
		  {e4, time_croma},
		  {e4, time_croma},
		  {e4, time_semicroma * 2},
		  {e4, time_croma},
		  {e4, time_croma},
		  {e4, time_semicroma * 2},
		  {e4, time_semicroma},
		  {g4, time_croma},
		  {c4, time_semicroma},
		  {d4, time_semicroma},
		  {e4, time_semicroma * 2},

		  // Oh, what fun it is to ride
		  {f4, time_croma},
		  {f4, time_croma},
		  {f4, time_croma},
		  {f4, time_croma},
		  {f4, time_semicroma},
		  {e4, time_semicroma},
		  {e4, time_croma},
		  {e4, time_croma},
		  {e4, time_croma},
		  {d4, time_croma},
		  {d4, time_croma},
		  {e4, time_croma},
		  {d4, time_croma * 2},
		  {g4, time_croma * 2},

		  // In a one-horse open sleigh!
		  {e4, time_croma},
		  {e4, time_croma},
		  {e4, time_semicroma * 2},
		  {e4, time_croma},
		  {e4, time_croma},
		  {e4, time_semicroma * 2},
		  {e4, time_semicroma},
		  {g4, time_croma},
		  {c4, time_semicroma},
		  {d4, time_semicroma},
		  {e4, time_semicroma * 2},

		  // Repeat the melody for other lines as necessary!
};
/* NOTE song[] =
{
	// 1
	{d3, time_semicroma},
	{d3, time_semicroma},
	{d4, time_croma},
	{a3, time_croma},
	{pause, time_semicroma},
	{a3b, time_semicroma},
	{pause, time_semicroma},
	{g3, time_croma},
	{f3, time_semicroma*2},
	{d3, time_semicroma},
	{f3, time_semicroma},
	{g3, time_semicroma},
	// 2
	{c3, time_semicroma},
	{c3, time_semicroma},
	{d4, time_croma},
	{a3, time_croma},
	{pause, time_semicroma},
	{a3b, time_semicroma},
	{pause, time_semicroma},
	{g3, time_croma},
	{f3, time_semicroma*2},
	{d3, time_semicroma},
	{f3, time_semicroma},
	{g3, time_semicroma},
	// 3
	{c3b, time_semicroma},
	{c3b, time_semicroma},
	{d4, time_croma},
	{a3, time_croma},
	{pause, time_semicroma},
	{a3b, time_semicroma},
	{pause, time_semicroma},
	{g3, time_croma},
	{f3, time_semicroma*2},
	{d3, time_semicroma},
	{f3, time_semicroma},
	{g3, time_semicroma},
	// 4
	{a2b, time_semicroma},
	{a2b, time_semicroma},
	{d4, time_croma},
	{a3, time_croma},
	{pause, time_semicroma},
	{a3b, time_semicroma},
	{pause, time_semicroma},
	{g3, time_croma},
	{f3, time_semicroma*2},
	{d3, time_semicroma},
	{f3, time_semicroma},
	{g3, time_semicroma},
	// 5

}; */

void TIMER3_IRQHandler(void)
{
	static int currentNote = 0;
	static int ticks = 0;
	if (!isNotePlaying())
	{
		++ticks;
		if (ticks == UPTICKS)
		{
			ticks = 0;
			playNote(song[currentNote++]);
		}
	}

	if (currentNote == (sizeof(song) / sizeof(song[0])))
	{
		currentNote = 0;
	}

	LPC_TIM3->IR = 1; /* clear interrupt flag */
	return;
}