#include "display.h"

void pause_screen()
{
	LCD_Clear(Blue);
	GUI_Text(120,160,(uint8_t*)"PAUSE",Black,White);
}

void print_pacman()
{
	// Reset background
	LCD_Clear(Blue);
	// Draw pacman
	LCD_DrawPacman(pacman.pos.x, pacman.pos.y, 3, Yellow);
}