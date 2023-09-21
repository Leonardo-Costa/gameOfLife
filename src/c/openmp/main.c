#include "gameoflife.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int row = 2048;
int col = 2048;

void runGameOfLife(int numThreads, int generations)
{
    float **grid;
    float **newGrid;

    clock_t start, end;
    double cpu_time_used;

    start = clock();
    allocateGrid(&grid, &newGrid);
    drawGlider(grid);
    drawPentomino(grid);
    loop(grid, newGrid, numThreads, generations);
    end = clock();
    cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;
    printf("%f", cpu_time_used);
}

int main(int argc, char *argv[])
{
    int numThreads = atoi(argv[1]);
    int generations = atoi(argv[2]);
    runGameOfLife(numThreads, generations);
    return 0;
}
