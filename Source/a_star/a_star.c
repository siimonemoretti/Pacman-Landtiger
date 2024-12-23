#include "a_star.h"

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

const int dx[NUM_DIRS] = {0, 1, 0, -1};
const int dy[NUM_DIRS] = {1, 0, -1, 0};

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

dir_t a_star(cell_t map[MAP_X][MAP_Y], int start_x, int start_y, int goal_x, int goal_y)
{
   // Priority queue
   priority_queue_t open;
   pq_init(&open, MAP_X * MAP_Y);

   // Visited array
   int visited[MAP_X][MAP_Y] = {0};
   dir_t directions[MAP_X][MAP_Y] = {0};
   int g_cost[MAP_X][MAP_Y] = {0};

   // Start node
   pq_push(&pq, (node_t){startX, startY, 0, heuristic(startX, startY, goalX, goalY)});
   visited[startX][startY] = 1;

   while (!pq_empty(&pq))
   {
      // Get node with the lowest f(x)
      node_t current = pq_pop(&pq);
      int x = current.x, y = current.y;

      // If we reach the goal, backtrack to find the first move
      if (x == goalX && y == goalY)
      {
         while (!(x == startX && y == startY))
         {
            dir_t dir = directions[x][y];
            x -= dx[dir];
            y -= dy[dir];

            // If we're back at the starting point, return the first direction
            if (x == startX && y == startY)
               return dir;
         }
      }

      // Explore neighbors
      for (int i = 0; i < NUM_DIRS; i++)
      {
         int nx = x + dx[i];
         int ny = y + dy[i];

         // Check if neighbor is valid (inside the map, not a wall or teleport)
         if (nx >= 0 && nx < MAP_X && ny >= 0 && ny < MAP_Y && map[nx][ny] != WALL && map[nx][ny] != TELEPORT)
         {
            int newG = current.g + 1; // Cost to reach this neighbor
            int newF = newG + heuristic(nx, ny, goalX, goalY);

            // If we find a better path to this cell, enqueue it
            if (!visited[nx][ny] || newG < g_cost[nx][ny])
            {
               pq_push(&pq, (node_t){nx, ny, newG, newF});
               visited[nx][ny] = 1;
               g_cost[nx][ny] = newG;
               directions[nx][ny] = (dir_t)i;
            }
         }
      }
   }
   // If no path found
   pq_free(&open);
   return NUM_DIRS; // Return an invalid direction
}