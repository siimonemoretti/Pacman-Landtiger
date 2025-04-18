#include "display.h"
#include <stdlib.h>
#include <stdio.h>

void pause_screen()
{
   GUI_Text((120 - (2 * 8) - 4), (160 - 8), (uint8_t *)"PAUSE",Yellow, BK_COLOR);
}

void lost_screen()
{
	GUI_Text((120 - (4 * 8) - 4), (160 - 8), (uint8_t *)"GAME OVER", Yellow, BK_COLOR);
}

void win_screen()
{
	GUI_Text((120 - (3 * 8) - 4), (160 - 8), (uint8_t *)"YOU WON", Yellow, BK_COLOR);
}

void draw_lifes()
{
   uint8_t i = 0;
	 // Draw a black line to cancel out previous hearts
	 LCD_DrawLine(8, 310, 40, 310, Black);
	 LCD_DrawLine(8, 309, 40, 309, Black);
	 LCD_DrawLine(8, 308, 40, 308, Black);
	 LCD_DrawLine(8, 307, 40, 307, Black);
	 LCD_DrawLine(8, 306, 40, 306, Black);
	 LCD_DrawLine(8, 305, 40, 305, Black);
	 LCD_DrawLine(8, 304, 40, 304, Black);
	 LCD_DrawLine(8, 303, 40, 303, Black);
	 for (; i < game.stats.lifes; i++)
   {
      LCD_DrawHeart(8 + i * 8, 310, Yellow);
   }
}

void draw_time_left_txt()
{
	GUI_Text(120 + 8, 3, (uint8_t*)"TIME:", White, BK_COLOR);
}

void draw_time_left()
{
	char str[3];  // 4 digits + null terminator
	sprintf(str, "%02d", game.stats.time_left);
	GUI_Text(120 + 8, 15, (uint8_t*)str, White, BK_COLOR);
}

void draw_cell(uint8_t x, uint8_t y, cell_t type)
{
	switch (type)
  {
  case PACMAN:
     LCD_DrawPacman(x, y, game.pacman.dir, game.pacman.color);
     break;
     break;
  case WALL:
     LCD_DrawWall(x, y, Blue);
	   break;
  case STANDARD_PILL:
     LCD_DrawStandardPill(x, y, Cyan);
     break;
  case POWER_PILL:
     LCD_DrawPowerPill(x, y, Magenta);
     break;
  case FLOOR:
		LCD_DrawFloor(x, y);
	case TELEPORT:
  default:
     break;
  }
}

void draw_map(game_t game)
{
   LCD_Clear(BK_COLOR);
	 uint8_t x, y;
   for (x = 0; x < MAP_X; x++)
      for (y = 0; y < MAP_Y; y++)
				draw_cell(x,y,game.map[x][y]);
   draw_lifes();
	 draw_time_left_txt();
}

void move_pacman(int16_t x, int16_t y)
{
	 // We may get an invalid y when we need to teleport
	 if (y == MAP_Y)
	 {
		  game.map[game.pacman.x][game.pacman.y] = TELEPORT;
      LCD_DrawFloor(game.pacman.x, game.pacman.y);
		  game.pacman.x = x;
      game.pacman.y = 0;
      game.map[game.pacman.x][game.pacman.y] = PACMAN;
      LCD_DrawPacman(game.pacman.x, game.pacman.y, game.pacman.dir, game.pacman.color);
		  return;
	 }
	 else if (y == -1)
	 {
		  game.map[game.pacman.x][game.pacman.y] = TELEPORT;
      LCD_DrawFloor(game.pacman.x, game.pacman.y);
		  game.pacman.x = x;
      game.pacman.y = MAP_Y - 1;
      game.map[game.pacman.x][game.pacman.y] = PACMAN;
      LCD_DrawPacman(game.pacman.x, game.pacman.y, game.pacman.dir, game.pacman.color);
		  return;
	 } 
   else if (x < 0 || x >= MAP_X || y < 0 || y >= MAP_Y) // Checks that x and y are valid
      return;
   switch (game.map[x][y])
   {
   case WALL:
      return;
   case FLOOR:
      game.map[game.pacman.x][game.pacman.y] = FLOOR;
      LCD_DrawFloor(game.pacman.x, game.pacman.y);
      game.pacman.x = x;
      game.pacman.y = y;
      game.map[game.pacman.x][game.pacman.y] = PACMAN;
      LCD_DrawPacman(game.pacman.x, game.pacman.y, game.pacman.dir, game.pacman.color);
      break;
   case STANDARD_PILL:
      game.map[game.pacman.x][game.pacman.y] = FLOOR;
			game.pill_counter--;
      LCD_DrawFloor(game.pacman.x, game.pacman.y);
      game.pacman.x = x;
      game.pacman.y = y;
      game.map[game.pacman.x][game.pacman.y] = PACMAN;
      LCD_DrawPacman(game.pacman.x, game.pacman.y, game.pacman.dir, game.pacman.color);
      can_message.score += 10;
      break;
   case POWER_PILL:
      game.map[game.pacman.x][game.pacman.y] = FLOOR;
			game.pill_counter--;
      LCD_DrawFloor(game.pacman.x, game.pacman.y);
      game.pacman.x = x;
      game.pacman.y = y;
      game.map[game.pacman.x][game.pacman.y] = PACMAN;
      LCD_DrawPacman(game.pacman.x, game.pacman.y, game.pacman.dir, game.pacman.color);
      can_message.score += 50;
	   /* Switch Ghost's mode to frightened */
		game.ghost.mode = FRIGHTENED_MODE;
	   game.ghost.frightened_cnt = 10;
      break;
	 case TELEPORT:
			game.map[game.pacman.x][game.pacman.y] = FLOOR;
      LCD_DrawFloor(game.pacman.x, game.pacman.y);
      game.pacman.x = x;
      game.pacman.y = y;
      game.map[game.pacman.x][game.pacman.y] = PACMAN;
      LCD_DrawPacman(game.pacman.x, game.pacman.y, game.pacman.dir, game.pacman.color);
   default:
      break;
   }
}