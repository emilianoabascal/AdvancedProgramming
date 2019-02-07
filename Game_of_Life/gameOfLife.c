/*
    Emiliano Abascal Gurria
    A01023234
    11/19/2018

    Simulation of Conway's Game of Life using OpenMP
    Based on the explanations at:
        https://en.wikipedia.org/wiki/Conway%27s_Game_of_Life
    Online versions:
        http://pmav.eu/stuff/javascript-game-of-life-v3.1.1/
        https://bitstorm.org/gameoflife/

*/

#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <omp.h>
#include "pgm_image.h"
// ADD YOUR EXTRA LIBRARIES HERE
#include "gameOfLife.h"

// ADD YOUR FUNCTION DECLARATIONS HERE

int main(int argc, char * argv[]){
    char * worldFile;
    char * mode;
    int iterations;
    printf("\n=== GAME OF LIFE ===\n");
    // Parsing the arguments
    if(argc == 4){
      if(isdigit(*argv[1])){
        iterations = atoi(argv[1]);
      }
      worldFile = argv[2];
      if(*argv[3] == 't' || *argv[3] == 'p' || *argv[3] == 'l'){
        mode = argv[3];
      }else{
        usage(argv[0]);
        return 1;
      }
    }else{
        usage(argv[0]);
        return 1;
    }
    // Run the game with the given arguments
    lifeSimulation(iterations, worldFile, mode);
    return 0;
}

// Print usage information for the program
void usage(const char * program){
    printf("Usage:\n");
    printf("%s [iterations] [world file] [t = Using Threads, p = Using OMP l = Using Linear Solution]\n", program);
}
// Main loop for the simulation
void lifeSimulation(int iterations, char * worldFile, char * mode){
  printf("world: '%s'\n#Of iterations: %d.\n", worldFile, iterations);
  world* lastWorld = getWorld(worldFile);//Get world from image.
  if(*mode == 't'){
    printf("Using threads\n");
  }else if(*mode == 'l'){
    printf("Using Linear\n");
  }else{
    printf("Using Paralelism\n");
  }
  double start = omp_get_wtime();
  for(int i = 0; i < iterations; i++){
      // Create Image with the last image generated.
      saveImage(lastWorld,i);
      // Generate a new image (world) with the same dimentions as the last image.
      world * current = createWorld(lastWorld->width, lastWorld->height);
      // Fill the generated image with the next elements.
      if(*mode == 't'){//Using Threads.
        solveWithThreads(lastWorld, current);
      }else if(*mode == 'p'){//Using OMP.
        solveWithParalelism(lastWorld, current);
      }else if(*mode == 'l'){//Using linear Solution.
        solveLinear(lastWorld, current);
      }
      // Free the last world.
      destroyWorld(lastWorld);
      // Make current world the last.
      lastWorld = current;
  }
  // Free the last world.
  destroyWorld(lastWorld);
  //Print runtime.
  printf("Time: %f\n", omp_get_wtime()-start);
}

// Save the contents of the world in a new image.
void saveImage(const world * world, int iteration){
  pgm_t worldImage;
  char buffer[50];
  //Create the image and save it in the Images directory.
  sprintf(buffer, "Images/%d.pgm", iteration);
  //Set the values for the image.
  strcpy(worldImage.magic_number, "P3");
  worldImage.image.width = world->width;
  worldImage.image.height = world->height;
  worldImage.max_value = 255;
  int r,g,b = 0;
  allocateImage(&worldImage.image);
  //Fill each pixel.
  for (int i=0; i<worldImage.image.height; i++){
      for (int j=0; j<worldImage.image.width; j++){
        if(world->pixels[i][j] == dead){
          r = 0;
          g = 250;
          b = 154;
        }else if(world->pixels[i][j] == Race1){
          r = 0;
          g = 0;
          b = 0;
        }else if(world->pixels[i][j] == Race2){
          r = 255;
          g = 51;
          b = 51;
        }else if(world->pixels[i][j] == empty){
          r = 255;
          g = 255;
          b = 255;
        }
      worldImage.image.pixels[i][j].value[R] = r;
      worldImage.image.pixels[i][j].value[G] = g;
      worldImage.image.pixels[i][j].value[B] = b;
      }
  }
  writePGMFile(buffer, &worldImage);
  freeImage(&worldImage.image);
}

world* createWorld(int x, int y){
  world* world = malloc(sizeof(world));
  world->width = x;
  world->height = y;
  world->pixels = malloc (world->height * sizeof(int *));
  world->pixels[0] = calloc (world->height * world->width, sizeof(int));
  for(int i = 0; i < world -> height; i++){
    world->pixels[i] = world->pixels[0] + world->width * i;
  }
  return world;
}
//Function to release memory of the world.
void destroyWorld(world* world){
    free(world->pixels[0]);
    free(world->pixels);
    world->width = 0;
    world->height = 0;
    free(world);
}
//Read an image and set it into a world.
world* getWorld(char* filename){
    int sizeX, sizeY, buffer = 0;//Declaration of necesary variables.
    //Read image.
    FILE* file = fopen(filename, "r");
    if (file == NULL){
        printf("File could not be opened!\n");
        exit(-1);
    }
    // Read sizes.
    fscanf(file, "%i %i\n", &sizeY, &sizeX);
    // Create world with that size.
    world* world = createWorld(sizeX, sizeY);
    // Read file into world
    for (int i = 0; i < sizeY; i++){
        for (int j = 0; j < sizeX; j++){
            fscanf(file, "%i", &buffer);
            // Transalte the numbers in the file to the pixel state statuses.
            if(buffer == 1){
              world->pixels[i][j] = Race1;
            }else if (buffer == 2){
              world->pixels[i][j] = Race2;
            }else if (buffer == 0){
              world->pixels[i][j] = empty;
            }
        }
    }
    //Close File
    fclose(file);
    //Return world.
    return world;
}

int countElement(world* world, int x, int y, int element){//Counts elements or gets average.
    int elementToGet = 0;
    for (int i = -1; i < 2; i++){
      for (int j = -1; j < 2; j++){
        if(i == 0 && j == 0){
          continue;
        }else{
          if (x + i >= 0 && y + j >= 0 && y + j < world->height && x + i < world->width){//detect the borders
            if(element == 0){//Get the color average
              if(world->pixels[y + j][x + i] == Race1){
                    elementToGet++;
              }
              if(world->pixels[y + j][x + i] == Race2){
                  elementToGet += 2;
              }
            }else if((world->pixels[y + j][x + i] == Race1 || world->pixels[y + j][x + i] == Race2) && element == 1){//Get neighbours
                   elementToGet++;
            }
          }
        }
      }
    }
    return elementToGet;
}
int nextState(world* world, int x, int y){//function to get the next state of the pixels.
    int status = world->pixels[y][x];
    int neighbours = countElement(world, x, y, 0);//Set the neighbours.
    int average = countElement(world, x, y, 1);//Set the average color.
    if (status == empty || status == dead){//If status is dead or empty, then if the neigbours are 3 and the average is less than 5, the race will be the first one, else will be the second.
        if (neighbours == 3){
            // Pixel is now Race1
            if(average < 5){
              return Race1;
            }else{
              return Race2;
            }
        }else{
            // Pixel state remains the same
            return status;
        }
    }else{// Pixel was the first tipe of race.
        if (neighbours < 2 || neighbours > 3){
            return dead;
        }else{// If 2 or 3 neighbours: let it live
          if(status == Race1){
            return Race1;
          }else{
            return Race2;
          }
        }
    }
}

// Function to solve the problems using threads, this implementation divides the rows between all the threads.
void solveWithThreads(world* previous, world* lastWorld){
    pthread_t* tIDs = malloc(MAX_THREADS * sizeof(pthread_t));
    int rowspth = (previous->height) / MAX_THREADS;
    if(rowspth != 0){
        for(int t = 0; t < MAX_THREADS-1; t++){
            createThread(lastWorld, previous, &tIDs[t], t*rowspth, (t+1)*rowspth);
        }
    }
    // Create a thread with the residual of the divition by the number of lines.
    createThread(lastWorld, previous, &tIDs[MAX_THREADS-1], (MAX_THREADS-1)*rowspth, previous->height);
    for(int y = 0; y < MAX_THREADS; y++){
        pthread_join(tIDs[y], NULL);
    }
    //Free thread.
    free(tIDs);
}
// Function to calculate the rows for the threads.
void* calculateRowsWithThreads(void* args){
    thread* data = (thread*)args;
    // Do all rows from row_init up until before row_end
    for (int y = data->rowStart; y < data->rowEnd; y++){
        for (int x = 0; x < (data->previous->width); x++){
            data->previous->pixels[y][x] = nextState(data->lastWorld, x, y);
        }
    }
    free(data);
    pthread_exit(0);
}

// Function to create threads.
void createThread(world* previous, world* lastWorld, pthread_t* tid, int yMin, int yMax){
    thread* data = malloc(sizeof(thread));
    data->rowStart = yMin;
    data->rowEnd = yMax;
    data->lastWorld = lastWorld;
    data->previous = previous;
    pthread_create(tid, NULL, calculateRowsWithThreads, (void*)data);
}
// Function to do the simulation using OMP
void solveWithParalelism(world* lastWorld, world* previous){
    int y, x;
    //Each pixel will be calculated using paralelism.
    #pragma omp parallel for default(none) shared(lastWorld, previous) private(y, x)
    for (y = 0; y < previous->height; y++){
        for (x = 0; x < previous->width; x++){
            previous->pixels[y][x] = nextState(lastWorld, x, y);
        }
    }
}
//Function to do the simulation linearly.
void solveLinear(world* lastWorld, world* previous){
  int y, x;
  for (y = 0; y < previous->height; y++){
    for (x = 0; x < previous->width; x++){
      previous->pixels[y][x] = nextState(lastWorld, x, y);
    }
  }
}
