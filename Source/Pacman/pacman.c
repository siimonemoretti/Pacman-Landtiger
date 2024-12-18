#include "pacman.h"

void init_random_pow_pills()
{
	uint16_t i;
	srand();
	for (i = 0; i < N_POW_PILLS; i++)
	{
		// We have 6 power pills, and we generate them before 30 s elapse
		game.power_pills[i] = (rand() % 5) + (5*i);
	}
}