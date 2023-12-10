/*Leonardo Loureiro Costa*/
#include "gameoflife.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <mpi.h>

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
  *grid = (float **)malloc(row * sizeof(float *));
  *newGrid = (float **)malloc(row * sizeof(float *));

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
  for (int i = 0; i < 50; i++)
  {
    for (int j = 0; j < 50; j++)
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

void computeDivision(float neighbors)
{
  float result = neighbors / 8;
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

  computeDivision(neighbors);

  return (float)neighbors;
}

void updateState(float ***grid, float ***newGrid, int numProcessors, int rank)
{
  int numberOfRow = row / numProcessors;
  int remainder = row % numProcessors;
  int currentRow = 0;

  int startRow = rank * numberOfRow;
  int endRow = startRow + numberOfRow;

  if (rank == numProcessors - 1)
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

int countLiveCells(float **grid)
{
  int count = 0;

  for (int i = 0; i < row; i++)
  {
    for (int j = 0; j < col; j++)
    {
      if (grid[i][j] == 1.0)
      {
        count += 1;
      }
    }
  }
  return count;
}

void loop(float **grid, float **newGrid, int numProcessors, int rank, int generations)
{
  for (int i = 0; i < generations; i++)
  {
    updateState(&grid, &newGrid, numProcessors, rank);

    // MPI_Barrier(MPI_COMM_WORLD);
    // if (rank == 0) {
    //   printGrid(newGrid);
    //   usleep(30000);         // pausa a tela
    //   printf("\033[H\033[J"); // limpa a tela
    // }
    // MPI_Barrier(MPI_COMM_WORLD);

    float **temp = grid;
    grid = newGrid;
    newGrid = temp;
  }
}