/*
    Emiliano Abascal Gurria
    A01023234
    11/19/2018
*/
#ifndef gameOfLife_H
#define gameOfLife_H

#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <time.h>
#include <omp.h>
#define MAX_THREADS 18
// Board struct
typedef struct worldStruct{
    int** pixels;
    int width;
    int height;
}world;
// Pixel status enum
typedef enum pixelStatus {
    empty,
    dead,
    Race1,
    Race2
} pStatus;
//Struct for passing data to threads
typedef struct threadData{
    world* lastWorld;
    world* previous;
    int rowStart;
    int rowEnd;
}thread;
//Declaration of functions
world* createWorld(int sizex, int sizey);
void destroyWorld(world* world);
world* getWorld(char* filename);
int nextState(world* world, int x, int y);
int countElement(world* world, int x, int y, int element);
void solveWithThreads(world* previous, world* last);
void createThread(world* previous, world* last, pthread_t* tid, int yMin, int yMax);
void* calculateRowsWithThreads(void* args);
void solveWithParalelism(world* last, world* current);
void solveLinear(world* last, world* previous);
void usage(const char * program);
void lifeSimulation(int iterations, char * start_file, char * mode);
void saveImage(const world * world, int iteration);
#endif