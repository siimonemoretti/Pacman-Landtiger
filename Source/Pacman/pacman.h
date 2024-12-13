#ifndef _PACMAN_H_
#define _PACMAN_H_

/* LCD Colors */
#define White 0xFFFF
#define Black 0x0000
#define Grey 0xF7DE
#define Blue 0x001F
#define Blue2 0x051F
#define Red 0xF800
#define Magenta 0xF81F
#define Green 0x07E0
#define Cyan 0x7FFF
#define Yellow 0xFFE0

// 28 Cells in X + 2*8 offset make up 240 pixels
#define MAP_X 28
#define MAP_Y 28
#define START_X 13
#define START_Y 23
#define X_OFFSET 8
#define Y_OFFSET 40
#define BK_COLOR Black

// Game's state: intro->gaming->intro (paused) and so on
typedef enum
{
	GAMESTATE_INTRO,
	GAMESTATE_GAME
} gamestate_t;

typedef enum
{
	RIGHT,	// 00
	DOWN,		// 01
	LEFT,		// 10
	UP,		// 11
	NUM_DIRS // 4 == The number of possible directions!
} dir_t;

typedef enum
{
	FLOOR, // Floor is 0 so we can init the array with 0's and that will mean floor
	PACMAN,
	STANDARD_PILL,
	POWER_PILL,
	WALL
} cell_t;

typedef struct
{
	uint8_t x;
	uint8_t y;
	dir_t dir;
	uint16_t color;
} pacman_t;

typedef struct
{
	pacman_t pacman;
	gamestate_t gamestate;
	cell_t map[MAP_X][MAP_Y];
} game_t;

#endif