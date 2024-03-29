/*
   Functions for working with arrays of characters
    Emiliano Abascal Gurria A01023234.
    Gilberto Echeverria
    gilecheverria@yahoo.com
    01/02/2017
*/

#ifndef STRING_FUNCTIONS_H
#define STRING_FUNCTIONS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Read a string using fgets, and remove the trailing '\n'
size_t inputString(char * string, int size, FILE * file_ptr);

#endif  /* NOT STRING_FUNCTIONS_H */
