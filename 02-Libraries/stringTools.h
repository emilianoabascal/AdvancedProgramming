/*
   Library with common functions to work with strings and files
   
   Gilberto Echeverria
   gilecheverria@yahoo.com
*/

#ifndef STRING_TOOLS_H
#define STRING_TOOLS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LENGTH 400

FILE * openAndCheck(char file_name[], char mode[]);
void clearStringEnter(char string[], int max_length);
void clearBufferEnter();
void getClean(char string[], int max_length);

#endif
