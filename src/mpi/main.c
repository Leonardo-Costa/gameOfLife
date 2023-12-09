/*Leonardo Loureiro Costa*/
#include "gameoflife.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <mpi.h>

int row = 2048;
int col = 2048;

void runGameOfLife(int generations, int size, int rank)
{
    float **grid;
    float **newGrid;
    float startTime, endTime;
    gameResult gameResult;

    clock_t start, end;
    double cpu_time_used;

    allocateGrid(&grid, &newGrid);
    drawGlider(grid);
    drawPentomino(grid);

    MPI_Barrier(MPI_COMM_WORLD);
    

    startTime = clock();
    loop(grid, newGrid, size, rank, generations);
    endTime = clock();

    float executionTime = (float)(endTime - startTime) / CLOCKS_PER_SEC;

    MPI_Barrier(MPI_COMM_WORLD);
    if (rank == 0) {
        printf("%f", executionTime);
    }
}

int main(int argc, char **argv)
{
    int size, rank;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int generations = 50;
    int numberOfLiveCells;

    runGameOfLife(generations, size, rank);

    MPI_Finalize();
    return 0;
}
