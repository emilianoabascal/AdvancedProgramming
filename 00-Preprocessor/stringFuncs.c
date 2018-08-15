#include "stringFuncs.h"
#include <string.h>
#include <stdio.h>


void readString(char* string, int stringSize){
	fgets(string, stringSize, stdin);
	string[strlen(string)-1] ='\0';
}