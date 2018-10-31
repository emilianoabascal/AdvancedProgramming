/*
   Library with common functions to work with strings and files
   
   Gilberto Echeverria
   gilecheverria@yahoo.com
*/

#include "stringTools.h"

// Try to open a file, and exit if there was an error
FILE * openAndCheck(char file_name[], char mode[])
{
	FILE * file_pointer = NULL;

	file_pointer = fopen(file_name, mode);
    // Exit the program if the file could not be opened
	if (file_pointer == NULL)
	{
		printf("Error: Could not open file '%s'!\n", file_name);
        exit(EXIT_FAILURE);
	}
	return file_pointer;
}

// Get rid of the newline after reading using fgets
void clearStringEnter(char string[], int max_length)
{
    // Change the last character in the string, if it is a '\n'
    if (strlen(string) < max_length-1 && string[strlen(string)-1] == '\n')
    {
        // Set it as a null character
        string[strlen(string)-1] = '\0';
    }
}

// Get rid of the newline in the input buffer
void clearBufferEnter()
{
    char garbage;

    // Discard the newline left over in the buffer
    // From: http://c-faq.com/stdio/stdinflush2.html
    while((garbage = getchar()) != '\n' && garbage != EOF)
        /* discard */ ;
}

// Read a string using fgets and get rid of trailing newlines before or after
void getClean(char string[], int max_length)
{
    // Read only the number of characters available
    fgets(string, max_length, stdin);
    // Get rid of the last newline
    clearStringEnter(string, max_length);
}
