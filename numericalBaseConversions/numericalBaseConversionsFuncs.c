/*

Emiliano Abascal Gurria A01023234

08/16/2018

File that contains all the functios necessary to convert any number with any base to another base.

*/
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>  
#include "numericalBaseConversionsFuncs.h"
/*

Function to convert any number with any base to base 10, receives the number that needs to be converted, the base of the number and the base that it will be converted to.

*/
void fromAnyBaseToDecimal(char *number, char *base1, char *base2){
	/*
	
	Declaration of all the necessary variables needed inside the function.
	
	'n' is the number of digits that the number contains.
	
	'b' is the original base of the number.
	
	*/
	
	
	int n = strlen(number)-1;
	int i = 0;
	int sum = 0;
	int temp;
	int b = atoi(base1);
	//End of variable declaration.
	
	/*
	
	This loop is used to take every character from the number and turn it into its decimal equivalent, then add it to the value of sum until it reaches the null character.
	
	if the original base is greater than 9, and the decimal value of the character being evaluated is greater than 57(this means that the character being evaluated is a number bigger than 9) then the temporary variable 'temp' will have the decimal value of the character minus 55, then the 'sum' value will be the operation to turn any base into base 10 plus the former value of sum. This comparison is used if the base is bigger than 9, for example if the number is hexadecimal and the number is 'A', 'A'-55 turns it into 10, i was able to get this information from an ascii table.
	
	if it wasn't the case, then the temporary variable 'temp' will have the decimal value of the character minus 48, and the 'sum' value will be the operation to turn any base into base 10 plus the former value of sum.
	
	*/
	
	while(number[i] != '\0'){
		if(b > 9 && number[i] > 57){
			temp = number[i] - 55;
			sum = sum + (temp)*(pow(b, n));
		}else{
			temp = number[i] - 48;
			sum = sum + (temp)*(pow(b, n));
		}
		n--;
		i++;
	}
	//End of loop.
	
	/*
	
	If the base that the number will be converted to is equal to 10 then the result is printed, if it wasn't, then the function fromDecimalToOtherBases is called passing the values of the number in base 10, and the base that the number will be converted to.
	
	*/
	if(atoi(base2) == 10){
		printf("Converting %s from base %s to base %s equals: %d.\n", number, base1, base2, sum);
	}else{
		printf("Converting %s from base %s to base %s equals: ", number, base1, base2);
		fromDecimalToOtherBases(sum, base2);
	}
	
}

/*

Function to convert the number in base 10 to the desired base, receives the number that needs to be converted and the base that it will be converted to.

*/

void fromDecimalToOtherBases(int number, char *base){
	
	/*
		
	Declaration of all the necessary variables needed inside the function.
		
	'n' is a copy of the number that will be needed to calculate the number of characters in the string of the resulting number.
		
	'b' is the base that is going to be converted to.
	
	'm' is an array of characters without initialization.
	
	*/
	
	int n = number;
	int b = atoi(base);
	int i = 0;
	char *m;
	
	/*
	
	This loop is used to know the size to calculate the number of characters of the array ('m') of the resulting number.
	
	*/
	
	while(n/b != 0){
		i++;
		n = n/b;
	}
	//End of loop.
	
	/*
		
	Now that the size is known, 'm' is initialized, using calloc to assign the number of elements that it will contain and the space in memory, so it can be freed later on.
		
	*/
	
	m = (char*)calloc(i, sizeof(char*));	
	
	/*
			
	This loop is used to assign the values of the 'mod' operation between the number and the base to the array ('m'), and then change the value of the number, this loop repeats until the iterator is the size of the array.
	
	When the loop is done, then the null char is added to the last element of the array.
	
	*/
		
	for(int j = 0; j <= i; j++){
		if(atoi(base) > 9){
			m[j] = number%b +'0';
		}else{
			m[j] = number%b +'0';
		}
		number = number/b;
	}
	//End of loop.
	m[i+1] = '\0';
	
	/*
				
	This loop is used to print every element of the array backwards, if the decimal value of the character being evaluated is greater than 57 then it prints the character + the 'BELL' character that has a value of 7, this is for numbers that have a greater base than 9, for example number in the array is 10, its decimal value is 58 which is equal to ':' if the 'BELL' character is added, then the value will be 65 which is equal to 'A'. Else it just prints the character in the array in the index position.
	
	When the loop is done, then a blank space is printed to make space for any other number that it will be converted, and 'm' is freed from memory.
	
	*/
	
	
	for(int j = i; j >= 0;j--){
		if(m[j] > 57){
			printf("%c", m[j] + '\a');//Bell Character
			
		}else{
			printf("%c", m[j]);
		}
	}
	
	
	
	printf(".\n");
	free(m);
}