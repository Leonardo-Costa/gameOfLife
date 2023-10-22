/*Leonardo Loureiro Costa*/
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
    gameResult gameResult;

    clock_t start, end;
    double cpu_time_used;

    allocateGrid(&grid, &newGrid);
    drawGlider(grid);
    drawPentomino(grid);
    gameResult = loop(grid, newGrid, numThreads, generations);
    printf("%f", gameResult.executionTime);
    return gameResult.numberOfLiveCells;
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
