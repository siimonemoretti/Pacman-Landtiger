#include "display.h"

void pause_screen()
{
	LCD_Clear(Blue);
	GUI_Text((120 - (2 * 8)), 160, (uint8_t *)"PAUSE", Black, White);
}

void draw_map()
{
	uint8_t x, y;
	for (x = 0; x < MAP_X; x++)
	{
		for (y = 0; y < MAP_Y; y++)
		{
			switch (map[x][y])
			{
			case PACMAN:
				LCD_DrawPacman(x, y, dir, pacman.color);
				break;
			case FLOOR:
				LCD_DrawFloor(x, y, Black);
				break;
			case WALL:
				LCD_DrawWall(x, y, White);
				break;
			case STANDARD_PILL:
				LCD_DrawStandardPill(x, y, White);
				break;
			case POWER_PILL:
				LCD_DrawPowerPill(x, y, White);
				break;
			default:
				break;
			}
		}
	}
}

void move_pacman(uint16_t x, uint16_t y)
{
	switch (map[x][y])
	{
	case WALL:
		return;
	case FLOOR:
		map[pacman.x][pacman.y] = FLOOR;
		LCD_DrawFloor(pacman.x, pacman.y);
		pacman.x = x;
		pacman.y = y;
		map[pacman.x][pacman.y] = PACMAN;
		LCD_DrawPacman(pacman.x, pacman.y, pacman.dir, pacman.color);
		break;
	case STANDARD_PILL:
		map[pacman.x][pacman.y] = FLOOR;
		LCD_DrawFloor(pacman.x, pacman.y);
		pacman.x = x;
		pacman.y = y;
		map[pacman.x][pacman.y] = PACMAN;
		LCD_DrawPacman(pacman.x, pacman.y, pacman.dir, pacman.color);
		// TODO: Update score
		break;
	case POWER_PILL:
		map[pacman.x][pacman.y] = FLOOR;
		LCD_DrawFloor(pacman.x, pacman.y);
		pacman.x = x;
		pacman.y = y;
		map[pacman.x][pacman.y] = PACMAN;
		LCD_DrawPacman(pacman.x, pacman.y, pacman.dir, pacman.color);
		// TODO: Update score
		break;
	default:
		break;
	}
}

void init_map_walls(cell_t map[MAP_X][MAP_Y])
{
   // Walls are predefined, layout is symmetric
   // Horizontal walls at top and bottom (row 0 and row MAP_Y - 1)
   uint8_t y;
   for (y = 0; y < MAP_Y; y++)
   {
      map[0][y] = WALL;
      map[MAP_X - 1][y] = WALL;
   }

   // 2+3 Next two rows have walls only at the borders
   map[1][0] = WALL;
   map[1][MAP_Y - 1] = WALL;
   map[2][0] = WALL;
   map[2][MAP_Y - 1] = WALL;
   map[MAP_X - 2][0] = WALL;
   map[MAP_X - 2][MAP_Y - 1] = WALL;
   map[MAP_X - 3][0] = WALL;
   map[MAP_X - 3][MAP_Y - 1] = WALL;
   // 4 Row
   map[3][0] = WALL;
   map[3][3] = WALL;
   map[3][4] = WALL;
   map[3][7] = WALL;
   map[3][20] = WALL;
   map[3][23] = WALL;
   map[3][24] = WALL;
   map[3][27] = WALL;
   map[MAP_X - 4][0] = WALL;
   map[MAP_X - 4][3] = WALL;
   map[MAP_X - 4][4] = WALL;
   map[MAP_X - 4][7] = WALL;
   map[MAP_X - 4][20] = WALL;
   map[MAP_X - 4][23] = WALL;
   map[MAP_X - 4][24] = WALL;
   map[MAP_X - 4][27] = WALL;
   // 5 Row
   map[4][0] = WALL;
   map[4][3] = WALL;
   map[4][4] = WALL;
   map[4][23] = WALL;
   map[4][24] = WALL;
   map[4][27] = WALL;
   map[MAP_X - 5][0] = WALL;
   map[MAP_X - 5][3] = WALL;
   map[MAP_X - 5][4] = WALL;
   map[MAP_X - 5][23] = WALL;
   map[MAP_X - 5][24] = WALL;
   map[MAP_X - 5][27] = WALL;
   // 6 Row: Borders + line from Y=10 to Y=17
   map[5][0] = WALL;
   map[5][27] = WALL;
   map[MAP_X - 6][0] = WALL;
   map[MAP_X - 6][27] = WALL;
   for (y = 10; y < 18; y++)
   {
      map[5][y] = WALL;
      map[MAP_X - 6][y] = WALL;
   }
   // 7+8 Row: Borders + line from Y=13 to Y=14
   map[6][0] = WALL;
   map[6][27] = WALL;
   map[7][0] = WALL;
   map[7][27] = WALL;
   map[MAP_X - 8][0] = WALL;
   map[MAP_X - 8][27] = WALL;
   map[MAP_X - 7][0] = WALL;
   map[MAP_X - 7][27] = WALL;
   for (y = 13; y < 15; y++)
   {
      map[6][y] = WALL;
      map[7][y] = WALL;
      map[MAP_X - 8][y] = WALL;
      map[MAP_X - 7][y] = WALL;
   }
   // 9 Row: Line of 5 starting from both borders, a wall in Y = 7 and Y = 20 and a line from 13 to 14
   for (y = 0; y < 5; y++)
   {
      map[8][y] = WALL;
      map[MAP_X - 9][y] = WALL;
      map[8][MAP_Y - 1 - y] = WALL;
      map[MAP_X - 9][MAP_Y - 1 - y] = WALL;
   }
   map[8][7] = WALL;
   map[8][20] = WALL;
   map[MAP_X - 9][7] = WALL;
   map[MAP_X - 9][20] = WALL;
   for (y = 13; y < 15; y++)
   {
      map[8][y] = WALL;
      map[MAP_X - 9][y] = WALL;
   }
   // 10+11 Row: wall @ 4, 7, 20, 23
   map[9][4] = WALL;
   map[9][7] = WALL;
   map[9][20] = WALL;
   map[9][23] = WALL;
   map[MAP_X - 10][4] = WALL;
   map[MAP_X - 10][7] = WALL;
   map[MAP_X - 10][20] = WALL;
   map[MAP_X - 10][23] = WALL;
   map[10][4] = WALL;
   map[10][7] = WALL;
   map[10][20] = WALL;
   map[10][23] = WALL;
   map[MAP_X - 11][4] = WALL;
   map[MAP_X - 11][7] = WALL;
   map[MAP_X - 11][20] = WALL;
   map[MAP_X - 11][23] = WALL;
   // 12 Row: same as 9
   for (y = 0; y < 5; y++)
   {
      map[11][y] = WALL;
      map[MAP_X - 12][y] = WALL;
      map[11][MAP_Y - 1 - y] = WALL;
      map[MAP_X - 12][MAP_Y - 1 - y] = WALL;
   }
   map[11][7] = WALL;
   map[11][20] = WALL;
   map[MAP_X - 12][7] = WALL;
   map[MAP_X - 12][20] = WALL;
   
   // 13 Row: Line from 12 to 15
   for (y = 12; y < 16; y++)
   {
      map[12][y] = WALL;
      map[MAP_X - 13][y] = WALL;
   }
   // 14 Row: Wall @ 11, 16
   map[13][11] = WALL;
   map[13][16] = WALL;
   map[MAP_X - 14][11] = WALL;
   map[MAP_X - 14][16] = WALL;

   // Set Pacman's position
   map[START_X][START_Y] = PACMAN;

   // Pills need to be unifomally distributed, and we need to have:
   // 6 power pills
   // 240 standard pills
   // Of course, we can place them only in FLOOR cells
   int seed = 0x1234;
   srand(seed);
   uint8_t num_power_pills = 0;
   uint8_t num_standard_pills = 0;
   while (num_power_pills < 6)
   {
      uint8_t x = rand() % MAP_X;
      uint8_t y = rand() % MAP_Y;
      if (map[x][y] == FLOOR)
      {
         map[x][y] = POWER_PILL;
         num_power_pills++;
      }
   }
   while (num_standard_pills < 240)
   {
      uint8_t x = rand() % MAP_X;
      uint8_t y = rand() % MAP_Y;
      if (map[x][y] == FLOOR)
      {
         map[x][y] = STANDARD_PILL;
         num_standard_pills++;
      }
   }
}