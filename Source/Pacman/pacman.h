#ifndef _PACMAN_H_
#define _PACMAN_H_

#define START_X	120
#define START_Y	160

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

typedef struct {
	uint16_t x;
	uint16_t y;
} int2_t;

typedef struct {
	int2_t pos;
	dir_t dir;
} pacman_t;

extern pacman_t pacman;
extern gamestate_t gamestate;

#endif