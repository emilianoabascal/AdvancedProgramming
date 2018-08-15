#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>  
#include "numericalBaseConversionsFuncs.h"

void fromAnyBaseToDecimal(char *number, char *base1, char *base2){
	int n = getNumberOfChars(number);
	int i = 0;
	int sum = 0;
	int temp;
	int b = atoi(base1);
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
	if(atoi(base2) == 10){
		printf("%s: %d\n", number, sum);
	}else{
		printf("%s: ", number);
		fromDecimalToOtherBases(sum, base2);
	}
	
}

void fromDecimalToOtherBases(int number, char *base){
	int n = number;
	int b = atoi(base);
	int i = 0;
	char *m;
	while(n/b != 0){
		i++;
		n = n/b;
	}
	m = (char*)calloc(i, sizeof(char*));
		
	for(int j = 0; j < i; j++){
		if(atoi(base) > 9){
			m[j] = number%b +'0';
		}else{
			m[j] = number%b +'0';
		}
		number = number/b;
	}
	
	m[i] = number%b+'0';
	m[i+1] = '\0'; 
	
	for(int j = i; j >= 0;j--){
		if(m[j] > 57){
			printf("%c", m[j] + '\a');//Bell Character
		}else{
			printf("%c", m[j]);
		}
	}
	printf("\n");
	free(m);
}


int getNumberOfChars(char *number){
	int n = 0;
	while(number[n] != '\0'){
		n++;
	}
	return n-1;
}
