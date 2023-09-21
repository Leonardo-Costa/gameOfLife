#include "gameoflife.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int row = 2048;
int col = 2048;

int runGameOfLife(int numThreads, int generations)
{
    int numberOfLiveCells;
    float **grid;
    float **newGrid;

    clock_t start, end;
    double cpu_time_used;

    start = clock();
    allocateGrid(&grid, &newGrid);
    drawGlider(grid);
    drawPentomino(grid);
    numberOfLiveCells = loop(grid, newGrid, numThreads, generations);
    end = clock();
    cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;
    printf("%f", cpu_time_used);
    return numberOfLiveCells;
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

int main(int argc, char *argv[])
{
    int numberOfLiveCells;
    int numThreads = atoi(argv[1]);
    int generations = atoi(argv[2]);
    char countCells = 'f';
    if (argc == 4)
    {
        countCells = argv[3][0];
    }

    numberOfLiveCells = runGameOfLife(numThreads, generations);

    if (countCells == 'y' || countCells == 'Y')
    {
        printf("\nNumero de celulas vivas: %d\n", numberOfLiveCells);
    }
    return 0;
}
