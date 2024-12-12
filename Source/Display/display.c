#include "display.h"

void pause_screen()
{
	LCD_Clear(Blue);
	GUI_Text((120-16),160,(uint8_t*)"PAUSE",Black,White);
}

void print_pacman()
{
	// Reset background
	LCD_Clear(Blue);
	// Draw pacman
	LCD_DrawPacman(pacman.pos.x, pacman.pos.y, 3, Yellow);
}

void draw_map()
{
	uint8_t x, y;
	for(x = 0; x < MAP_X; x++)
	{
		for(y = 0; y < MAP_Y; y++)
		{
			switch(map[x][y])
			{
				
			}
		}
	}
}