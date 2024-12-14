#include "display.h"
#include <stdlib.h>

void pause_screen()
{
   GUI_Text((120 - (2 * 8) - 4), (160 - 8), (uint8_t *)"PAUSE", Black, White);
}

void draw_lifes()
{
   uint8_t i = 0;
   for (; i < game.lifes; i++)
   {
      LCD_DrawHeart(5 + i * 8, 310, Red);
   }
}

void draw_map(game_t game)
{
   LCD_Clear(BK_COLOR);
   uint8_t x, y;
   for (x = 0; x < MAP_X; x++)
   {
      for (y = 0; y < MAP_Y; y++)
      {
         switch (game.map[x][y])
         {
         case PACMAN:
            LCD_DrawPacman(x, y, game.pacman.dir, game.pacman.color);
            break;
         case FLOOR:
				 case TELEPORT:
            // LCD_DrawFloor(x, y);
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
         default:
            break;
         }
      }
   }
   draw_lifes();
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
      LCD_DrawFloor(game.pacman.x, game.pacman.y);
      game.pacman.x = x;
      game.pacman.y = y;
      game.map[game.pacman.x][game.pacman.y] = PACMAN;
      LCD_DrawPacman(game.pacman.x, game.pacman.y, game.pacman.dir, game.pacman.color);
      game.score += 10;
      break;
   case POWER_PILL:
      game.map[game.pacman.x][game.pacman.y] = FLOOR;
      LCD_DrawFloor(game.pacman.x, game.pacman.y);
      game.pacman.x = x;
      game.pacman.y = y;
      game.map[game.pacman.x][game.pacman.y] = PACMAN;
      LCD_DrawPacman(game.pacman.x, game.pacman.y, game.pacman.dir, game.pacman.color);
      game.score += 50;
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