#include "a_star.h"

void pq_init(priority_queue_t *pq, int capacity)
{
   pq->node = (node_t *)malloc(capacity * sizeof(node_t));
   pq->size = 0;
   pq->capacity = capacity;
}

void pq_push(priority_queue_t *pq, node_t *node)
{
   if (pq->size == pq->capacity)
   {
      return;
   }
   pq->node[pq->size++] = *node;
   int i = pq->size - 1;
   while (i > 0)
   {
      int parent = (i - 1) / 2;
      if (pq->node[i].f >= pq->node[parent].f)
      {
         break;
      }
      node_t tmp = pq->node[i];
      pq->node[i] = pq->node[parent];
      pq->node[parent] = tmp;
      i = parent;
   }
}

node_t pq_pop(priority_queue_t *pq)
{
   node_t ret = pq->node[0];
   pq->node[0] = pq->node[--pq->size];
   int i = 0;
   while (i * 2 + 1 < pq->size)
   {
      int a = i * 2 + 1;
      int b = i * 2 + 2;
      if (b < pq->size && pq->node[b].f < pq->node[a].f)
      {
         a = b;
      }
      if (pq->node[i].f <= pq->node[a].f)
      {
         break;
      }
      node_t tmp = pq->node[i];
      pq->node[i] = pq->node[a];
      pq->node[a] = tmp;
      i = a;
   }
   return ret;
}

int pq_empty(priority_queue_t *pq)
{
   return pq->size == 0;
}

void pq_free(priority_queue_t *pq)
{
   free(pq->node);
}

int heuristic(int x1, int y1, int x2, int y2)
{
   return abs(x1 - x2) + abs(y1 - y2);
}

static int dy[4] = {0, 1, 0, -1};
static int dx[4] = {1, 0, -1, 0};

dir_t a_star(cell_t map[MAP_X][MAP_Y], int start_x, int start_y, int goal_x, int goal_y)
{
   // Priority queue
   priority_queue_t pq;
   pq_init(&pq, MAP_X * MAP_Y);

   // Visited array
   int visited[MAP_X][MAP_Y] = {0};
   dir_t directions[MAP_X][MAP_Y] = {0};
   int g_cost[MAP_X][MAP_Y] = {0};

   // Start node
	 node_t n = {start_x, start_y, 0, heuristic(start_x, start_y, goal_x, goal_y)};
   pq_push(&pq, &n);
   visited[start_x][start_y] = 1;

   while (!pq_empty(&pq))
   {
      // Get node with the lowest f(x)
      node_t current = pq_pop(&pq);
      int x = current.x, y = current.y;

      // If we reach the goal, backtrack to find the first move
      if (x == goal_x && y == goal_y)
      {
         while (!(x == start_x && y == start_y))
         {
            dir_t dir = directions[x][y];
            x -= dx[dir];
            y -= dy[dir];

            // If we're back at the starting point, return the first direction
            if (x == start_x && y == start_y)
						{
							 pq_free(&pq);
               return dir;
						}
         }
      }

      // Explore neighbors
			int i;
      for (i = 0; i < NUM_DIRS; i++)
      {
         int nx = x + dx[i];
         int ny = y + dy[i];

         // Check if neighbor is valid (inside the map, not a wall or teleport)
         if (nx >= 0 && nx < MAP_X && ny >= 0 && ny < MAP_Y && map[nx][ny] != WALL && map[nx][ny] != TELEPORT)
         {
            int newG = current.g + 1; // Cost to reach this neighbor
            int newF = newG + heuristic(nx, ny, goal_x, goal_y);

            // If we find a better path to this cell, enqueue it
            if (!visited[nx][ny] || newG < g_cost[nx][ny])
            {
							 node_t n = {nx, ny, newG, newF};
               pq_push(&pq, &n);
               visited[nx][ny] = 1;
               g_cost[nx][ny] = newG;
               directions[nx][ny] = (dir_t)i;
            }
         }
      }
   }
   // If no path found
   pq_free(&pq);
   return NUM_DIRS; // Return an invalid direction
}