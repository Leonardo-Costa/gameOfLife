#ifndef GAMEOFLIFE_H
#define GAMEOFLIFE_H

extern int row;
extern int col;

void fillGrid(float ***grid);
void allocateGrid(float ***grid, float ***newGrid);
void printGrid(float **grid);
void drawGlider(float **grid);
void drawPentomino(float **grid);
int getNeighbors(float **grid, int i, int j);
void updateState(float ***grid, float ***newGrid, int numThreads);
int loop(float **grid, float **newGrid, int numThreads, int generations);
int countLiveCells(float **grid);
void computeDivision(float neighbors);

#endif // GAMEOFLIFE_H
