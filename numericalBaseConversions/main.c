/*

Emiliano Abascal Gurria A01023234

08/16/2018

Program that can convert numbers from base 10 (decimal) to other bases, and also from other bases to decimal.

main file in which the main function is located and the program is initialized.

*/
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <unistd.h>
#include "numericalBaseConversionsFuncs.h"
int main(int argc, char **argv) {
	
	/*
		
	Declaration of all the necessary variables needed inside the main function.
		
	'c' is the number that will be returned by the getopt function.
		
	'base1' is the string that contains the number of the base that the number introduced will have.
	
	'base2' is the string that contains the number of the base that the number introduced be converted to.
	
	'i' is an iterator
		
	*/
	
	int c;
	char *base1;
	char *base2;
	int i = 0;
	
	/*
	
	This loop is to check the arguments and options introduced when running thre program, inside there is a switch that analizes the value of c, when one of the options is 'i' then the value after is going to be the 'base1' string, if the option is 'o' then the next value is stored on 'base2'.
	
	*/
	
	while ((c = getopt(argc, argv, "io:")) != -1)
		switch (c){
			case 'i':
				base1 = argv[optind];
				optind++;
				break;
			case 'o':	
				base2 = argv[optind -1];
				
				/*
				
				This loop is used to read every number that the user wants to convert and calls the function 'fromAnyBaseToDecimal' passing it the values of 'base1', 'base2' and the number.
				
				*/
				
				for(i = optind; i <= argc-1; i++){
					fromAnyBaseToDecimal(argv[i], base1, base2);
				}
				break;
			default:
				return 1;
				abort ();
			}
		return 0;
}