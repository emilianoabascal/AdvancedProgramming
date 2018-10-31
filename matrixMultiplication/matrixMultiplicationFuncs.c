/*

Emiliano Abascal Gurria A01023234

08/27/2018

File that contains all the functios necessary to multiply and manage the matrixes given by the user.

*/

#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include "matrixMultiplicationFuncs.h"

/*

Function to get the matrix information out of the .txt file so it can be multiplied.

*/
matrix* readFile(char file1[]){
  
  /*
    
    Declaration of all the necessary variables needed inside the function.
    
    'f' is the file that is going to be read.
    
    'm' is a pointer to a structure of type Matrix, which has a 2 dimentional array, the number of columns and the number of rows, it is used to store a matrix.
    
    'buffer' is a string in which every line of the file will be temporarely stored.
    
    'temporal' is a temporal character for every character of the line.
    
    'i' is an iterator.
    
  */
  
  FILE *f;
  matrix * m;
  char buffer[255];
  char*temporal;
  int i = 0;
  //End of variable declaration.
  
  /*
  
  memory is allocated for matrix 'm', the file is opened, the first line of the file is assigned to 'buffer' and the number of rows inside 'm' is pulled from the first number inside the first line of the buffer.
  
  */
  m = (matrix*) malloc(sizeof(matrix));
  f = fopen(file1, "r");
  fgets(buffer, sizeof buffer, f);
  
  m->rows = atoi(&buffer[0]);
  
  /*
  
  The loop is used to get the number of columns that the matrix will have, it goes from the start of the buffer until it reaches the null character or it fouds the number of columns, and if the buffer in the 'i' position is equal to an empty space, then the number of columns will be the number right after the space.
  
  */
  while (buffer[i] != '\0'){
    if(buffer[i] == ' '){
      m->columns = atoi(&buffer[i+1]);
      break;//end of loop
    }
    i++;
  }//end of loop
  
  /*
  
  Memory is allocated for the array inside of the matrix type.
  
  */

  m-> array = (double**) calloc(m->rows, sizeof(double));
  
  /*
  
  The loop is used to asign memory space to each of the columns of the Matrix, also to assign the values to the matrix itself. it goes from 0 to the number of rows, then there is another loop which goes from 0 to the number of columns, then the array in the index [i][j] is assigned the value of the next number in the buffer if it is not the first. In the use of the function 'strtof' i would like to thank and give credit to Enrique Lira Martinez, who expalined me how the function worked.
  
  When the loop is done, then the file is closed and 'm' is returned.
  
  */
  
  
  for(int i = 0; i < m->rows; i++){
    m-> array[i] = (double*) calloc(m->columns, sizeof(double));
    fgets(buffer, sizeof buffer, f);
    temporal = buffer;
    for(int j = 0; j < m->columns;j++){
      m->array[i][j] = strtof(temporal, &temporal);
    }//End of loop
  }//End of loop
  fclose(f);
  return m;
}


/*

Function to multiply two matrixes.

*/
void multipication(matrix * m1, matrix * m2){
  
  /*
    
    Declaration of all the necessary variables needed inside the function.
    
    'result' is a pointer to a structure of type Matrix, which has a 2 dimentional array, the number of columns and the number of rows, it is used to store a matrix, in this case the resulting matrix.
    
    'f' is the resulting file in which the result will be written.
        
  */
  
  matrix * result = (matrix*) malloc (sizeof(matrix));
  FILE *f = fopen("output.txt", "w"); 
  
  /*
  
  The number of rows in result will be the same as in m1 and the number of columns as in m2.
  
  */
  
  result -> rows = m1 -> rows;
  result -> columns = m2 -> columns;
  
  /*
  
  Memory is allocated for the array inside of the matrix type and the number of rows and columns inside of result will be printed inside the output.txt file.
  
  */
  
  result -> array = (double**) calloc (result -> rows, sizeof(double));
  
  fprintf(f, "%i %i\n", result -> rows, result -> columns);
  
  /*
  
  The loop is used to asign memory space to each of the columns of the Matrix, also to assign the result of the multiplication to the result array inside of result. The loop goes from 0 to the number of rows of result, then there are two other loops, one which goes from 0 to the number of columns of. m2 and anotherone inside which goes from 0 to the number of columns of m1. then the result is the original value of the array inside result in the position [i][j] plus the multiplication of the values of the matrix 1 in position [i][k] and the values of the matrix 2 in position [k][j]. When the operation is done then the result of it is printed in the output.txt, when this is done j times, then a '\n' is printed on the output.txt file.
  
  */
  
  for(int i = 0; i < result -> rows; i++){
    result -> array[i] = (double*) calloc(result->columns, sizeof(double));
    for(int j = 0; j < m2 -> columns; j++){
      for(int k = 0; k < m1 -> columns; k++){
        result -> array[i][j] += m1 -> array[i][k] * m2 -> array[k][j];
      }//end of loop
      fprintf(f,"%f  ", result ->array[i][j]);
    }//end of loop
    fprintf(f, "\n");
  }//end of loop
  fclose(f);//The File is closed.
  cleanMemory(result);//The 'cleanMemory' is called to erase the 'result' matrix.
}

/*

Function to clear memory space of any matrix type.

*/

void cleanMemory(matrix * m){
  
  /*
  
  This loop is used to free memory space of each of the spaces of the 2d array inside the matrix type.
  
  */
  
  for(int i = 0; i < m -> rows; i++){
    free(m -> array[i]);
  }//end of loop.
  /*
  
    The array in m is erased then the Matrix type itself is erased.
  
  */
  free (m -> array);
  free(m);
}
