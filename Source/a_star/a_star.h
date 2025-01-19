#ifndef _A_STAR_H_
#define _A_STAR_H_

/**
 * @brief A* algorithm implementation for PacMan ghost movement
 * Heuristic: Manhattan distance
 * Supposing ghost won't teleport nor go through walls
 */

#include <stdlib.h>
#include "../Pacman/pacman.h"

typedef struct
{
   int x;
   int y;
   int g; // cost from start to this node
   int f; // total cost of this node (= g + h)
} node_t;

typedef struct
{
   node_t *node;
   int size;
   int capacity;
} priority_queue_t;

void pq_init(priority_queue_t *pq, int capacity);
void pq_free(priority_queue_t *pq);
void pq_push(priority_queue_t *pq, node_t *node);
node_t pq_pop(priority_queue_t *pq);
int pq_empty(priority_queue_t *pq);
int heuristic(int x1, int y1, int x2, int y2);
dir_t a_star(cell_t map[MAP_X][MAP_Y], int start_x, int start_y, int goal_x, int goal_y);

#endif