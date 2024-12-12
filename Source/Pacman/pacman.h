#ifndef _PACMAN_H_
#define _PACMAN_H_

#define START_X	120
#define START_Y	160
#define MAP_X		28
#define MAP_Y 	28

// Game's state: intro->gaming->intro (paused) and so on
typedef enum {
	GAMESTATE_INTRO,
	GAMESTATE_GAME
} gamestate_t;

typedef enum {
	RIGHT,				// 00
	DOWN,					// 01
	LEFT,					// 10
	UP, 					// 11
	NUM_DIRS			// 4 == The number of possible directions!
} dir_t;

typedef enum {
	PACMAN,
	FLOOR,
	STANDARD_PILL,
	POWER_PILL,
	WALL
} cell_t;

typedef struct {
	uint8_t x;
	uint8_t y;
} int2_t;

typedef struct {
	int2_t pos;
	dir_t dir;
} pacman_t;

extern pacman_t pacman;
extern gamestate_t gamestate;
extern cell_t map[MAP_X][MAP_Y];

#endif