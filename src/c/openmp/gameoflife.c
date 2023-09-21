#include "gameoflife.h"
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <omp.h>

struct ThreadData
{
  float ***grid;
  float ***newGrid;
  int start_row;
  int end_row;
};

void fillGrid(float ***grid)
{
  for (int i = 0; i < row; i++)
  {
    for (int j = 0; j < col; j++)
    {
      (*grid)[i][j] = 0.0;
    }
  }
}

void allocateGrid(float ***grid, float ***newGrid)
{
  // aloca memória para o array de ponteiros
  *grid = (float **)malloc(row * sizeof(float *));
  *newGrid = (float **)malloc(row * sizeof(float *));

  // cada ponteiro do array de ponteiros aponta para outro array de ponteiros
  // que apontam para um float
  for (int i = 0; i < row; i++)
  {
    (*grid)[i] = (float *)malloc(col * sizeof(float));
    (*newGrid)[i] = (float *)malloc(col * sizeof(float));
  }

  fillGrid(grid);
  fillGrid(newGrid);
}

void printGrid(float **grid)
{
  for (int i = 0; i < row; i++)
  {
    for (int j = 0; j < col; j++)
    {
      if (grid[i][j] == 0.0)
      {
        printf(".");
      }
      else
      {
        printf("0");
      }
    }
    printf("\n");
  }
}

void drawGlider(float **grid)
{
  int y = 1, x = 1;
  grid[y][x + 1] = 1.0;
  grid[y + 1][x + 2] = 1.0;
  grid[y + 2][x] = 1.0;
  grid[y + 2][x + 1] = 1.0;
  grid[y + 2][x + 2] = 1.0;
}

void drawPentomino(float **grid)
{
  int y = 10, x = 30;
  grid[y][x + 1] = 1.0;
  grid[y][x + 2] = 1.0;
  grid[y + 1][x] = 1.0;
  grid[y + 1][x + 1] = 1.0;
  grid[y + 2][x + 1] = 1.0;
}

int getNeighbors(float **grid, int i, int j)
{
  int neighbors = 0;

  int dx[] = {-1, -1, -1, 0, 0, 1, 1, 1};
  int dy[] = {-1, 0, 1, -1, 1, -1, 0, 1};

  for (int k = 0; k < 8; k++)
  {
    int x = (i + dx[k] + row) % row;
    int y = (j + dy[k] + col) % col;

    if (grid[x][y] != 0.0)
    {
      neighbors++;
    }
  }

  return neighbors;
}

void updateState(float ***grid, float ***newGrid, int numThreads)
{
  int numberOfRow = row / numThreads;
  int remainder = row % numThreads;
  int currentRow = 0;

#pragma omp parallel num_threads(numThreads) shared(grid, newGrid, numberOfRow, remainder, currentRow)
  {
    int threadID = omp_get_thread_num();
    int startRow = threadID * numberOfRow;
    int endRow = startRow + numberOfRow;

    if (threadID == numThreads - 1)
    {
      endRow += remainder;
    }

    for (int i = startRow; i < endRow; i++)
    {
      for (int j = 0; j < col; j++)
      {
        int neighbours = getNeighbors(*grid, i, j);
        if ((*grid)[i][j] == 1)
        {
          if (neighbours < 2)
          {
            (*newGrid)[i][j] = 0;
          }
          else if (neighbours == 2 || neighbours == 3)
          {
            (*newGrid)[i][j] = 1;
          }
          else if (neighbours >= 4)
          {
            (*newGrid)[i][j] = 0;
          }
        }
        else
        {
          if (neighbours == 3)
          {
            (*newGrid)[i][j] = 1;
          }
          else
          {
            (*newGrid)[i][j] = 0;
          }
        }
      }
    }
  }
}

void loop(float **grid, float **newGrid, int numThreads, int generations)
{
  for (int i = 0; i < generations; i++)
  {
    updateState(&grid, &newGrid, numThreads);

    float **temp = grid;
    grid = newGrid;
    newGrid = temp;
  }
}