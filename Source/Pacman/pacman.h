#ifndef _PACMAN_H_
#define _PACMAN_H_
#include <stdint.h>

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
#define START_X 11
#define START_Y 13
#define X_OFFSET 8
#define Y_OFFSET 40
#define BK_COLOR Black
#define N_POW_PILLS 6

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
	UP,		  // 11
	NUM_DIRS // 4 == The number of possible directions!
} dir_t;

typedef enum
{
	FLOOR = 0, // Floor is 0 so we can init the array with 0's and that will mean floor
	PACMAN,
	STANDARD_PILL,
	POWER_PILL,
	WALL,
	TELEPORT
} cell_t;

typedef struct
{
	int16_t x;
	int16_t y;
	dir_t dir;
	uint16_t color;
} pacman_t;

typedef enum
{
	CHASE_MODE,
	FRIGHTENED_MODE
} ghost_mode_t;

static const uint8_t ghost[6][6] = {
  {0, 0, 1, 1, 0, 0},
	{0, 1, 1, 1, 1, 0},
	{1, 2, 1, 1, 2, 1},
	{1, 1, 1, 1, 1, 1},
	{1, 1, 1, 1, 1, 1},
  {1, 0, 1, 1, 0, 1},
};

typedef struct
{
	int16_t x;
	int16_t y;
	ghost_mode_t mode;
	int8_t visible;
	int8_t frightened_cnt;
} ghost_t;

typedef struct
{
	int8_t time_left;
	uint8_t lifes;
	uint16_t score;
} can_msg_t;

typedef struct
{
	ghost_t ghost;
	pacman_t pacman;
	can_msg_t stats;
	gamestate_t gamestate;
	uint16_t pill_counter;
	cell_t map[MAP_X][MAP_Y];
	uint16_t power_pills[N_POW_PILLS];
} game_t;

void init_random_pow_pills();
void move_ghost();

extern game_t game;

#endif