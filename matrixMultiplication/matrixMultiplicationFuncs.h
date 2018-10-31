/*

Emiliano Abascal Gurria A01023234

08/16/2018

This file is used to declare de functions to use in the matrixMultiplicationFuncs.c

*/


#ifndef matrixMultiplicationFuncs_H
#define matrixMultiplicationFuncs_H

/*

A structure M is defined to keep the array and the number of columns and rows it has.

*/

typedef struct M{
  double ** array;
  int rows;
  int columns;
}matrix;

/*Declaration of functions*/

matrix* readFile(char file1[]);
void multipication(matrix * m1, matrix * m2);
void cleanMemory(matrix * m);
#endif
