/*

Emiliano Abascal Gurria A01023234

08/27/2018

Program that multiplies two matrixes.

main file in which the main function is located and the program is initialized.

The program should be ran like this: './matrixMultiplication -i matrix1.txt matrix2.txt' and it will return an output.txt with the multiplication result.

*/
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <unistd.h>
#include "matrixMultiplicationFuncs.h"

int main(int argc, char **argv) {
  
  /*
      
    Declaration of all the necessary variables needed inside the main function.
      
    'c' is the number that will be returned by the getopt function.
      
    'm1' is a pointer to a structure of type Matrix, which has a 2 dimentional array, the number of columns and the number of rows, it is used to store a matrix.
    
    'm2' is a pointer to a structure of type Matrix, which has a 2 dimentional array, the number of columns and the number of rows, it is used to store a matrix.
          
  */
  
    matrix * m1;
    matrix * m2;
    int c;
    
    /*
      
      This loop is to check the arguments and options introduced when running thre program, inside there is a switch that analizes the value of c, when one of the options is 'i' then m1 and m2 are set by the returning values of the function 'readFile' with the name of the files passed as arguments.
      
    */



    while ((c = getopt(argc, argv, "io:")) != -1){
		switch (c){
			case 'i':
        m1 = readFile(argv[optind]);
        m2 = readFile(argv[optind + 1]);
				break;
			default:
				return 1;
				abort ();
			}
    }//End of loop
    
    /*
    
    if the number of columns in the first matrix is equal to the number of rows in the second, then the function 'multiplication' is called passing it the two matrixes.
    
    if not, then the multiplication can not be done and a message is printed to the user.
    
    */
    
    if(m1->columns == m2->rows){
      multipication(m1, m2);
    }else{
      printf("The multipication can not be done.\n");
    }//End of condition
    
    /*
    
    The function 'cleanMemory' is called passing it the two matrixes that are going to be deleted.
    
    */
    cleanMemory(m1);
    cleanMemory(m2);


  return 0;
}
