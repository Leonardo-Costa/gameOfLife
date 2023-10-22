#include "gameoflife.h"
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

struct ThreadData
{
  float ***grid;
  float ***newGrid;
  int start_row;
  int end_row;
};

void *updateStateThread(void *arg)
{
  struct ThreadData *data = (struct ThreadData *)arg;
  float ***grid = data->grid;
  float ***newGrid = data->newGrid;

  for (int i = data->start_row; i < data->end_row; i++)
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

  pthread_exit(NULL);
}

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

  return neighbors;
}

void updateState(float ***grid, float ***newGrid, int numThreads)
{
  // alocando um array de threads
  pthread_t threads[numThreads];

  // alocando um array de structs com os dados para threads
  struct ThreadData threadData[numThreads];

  int numberOfRow =
      row / numThreads; // quantas linhas da matriz cada thread vai processar
  int remainder =
      row %
      numThreads; // quantas linhas vao sobrar no caso da divisao resultar em resto
  int currentRow = 0;

  for (int i = 0; i < numThreads; i++)
  {
    threadData[i].grid = grid;
    threadData[i].newGrid = newGrid;
    threadData[i].start_row = currentRow;
    currentRow += numberOfRow;
    if (i == numThreads - 1)
    {
      threadData[i].end_row = currentRow + remainder;
    }
    else
    {
      threadData[i].end_row = currentRow;
    }

    // criando a thread e passando os parametros: o endereco de memoria, o attr,
    // a rotina e argumentos, a struct de data.
    pthread_create(&threads[i], NULL, updateStateThread, &threadData[i]);
  }

  // join para que as threads aguardem todas finalizarem para prosseguir com o
  // codigo
  for (int i = 0; i < numThreads; i++)
  {
    pthread_join(threads[i], NULL);
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

int loop(float **grid, float **newGrid, int numThreads, int generations)
{
  for (int i = 0; i < generations; i++)
  {
    updateState(&grid, &newGrid, numThreads);
    // printGrid(newGrid);
    // usleep(30000);         // pausa a tela
    // printf("\033[H\033[J"); // limpa a tela

    float **temp = grid;
    grid = newGrid;
    newGrid = temp;
  }

  return countLiveCells(newGrid);
}