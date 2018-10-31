/*
    Program using custom library files to test compiling and linking
    
    Gilberto Echeverria
    gilecheverria@yahoo.com
*/

#include <math.h>
#include "stringTools.h"

#define STRING_SIZE 50

///// FUNCTION DECLARATIONS /////
void menu();
int isValidFile(char file_name[]);
void appendToFile(char file_name[]);
void readFile(char file_name[]);
void printSquareRoots();

int main()
{
    printf("\nSTRING TEST PROGRAM\n");
    
    // Only call the menu function
    menu();
    
    return 0;
}

///// FUNCTION DEFINITIONS /////

// Loop for the user interface
void menu()
{
    char ans = 'a';
    char file_name[STRING_SIZE] = "";
    
    while (ans != 'q')
    {
        printf("\nUser menu:\n");
        printf("\tn. Select the name of the file to work with\n");
        printf("\ta. Add text to the file\n");
        printf("\tp. Print the contents of the file\n");
        printf("\tm. Print a table of square roots\n");
        printf("\tq. Quit the program\n");
        printf("Select an option: ");
        scanf("%c", &ans);
        // Get rid of the '\n' character that the user had to type
        clearBufferEnter();
        
        switch (ans)
        {
            case 'n':
                printf("Enter the name of the file: ");
                getClean(file_name, STRING_SIZE);
                break;
            case 'a':
                if (isValidFile(file_name))
                {
                    appendToFile(file_name);
                }
                break;
            case 'p':
                if (isValidFile(file_name))
                {
                    readFile(file_name);
                }
                break;
            case 'm':
                printSquareRoots();
                break;
            case 'q':
                printf("Thanks for using the program. Bye!\n");
                break;
            default:
                printf("Incorrect option. Try again ...\n");
        }
    }
}

// Check that the name of the file is valid
// At the moment it only checks that the length of the string is greater than 0
int isValidFile(char file_name[])
{
    if (strlen(file_name) > 0)
        return 1;
    // Print a warning message if the filename has 0 length
    printf("Invalid file name. Indicate the file to use first\n");
    return 0;
}

// Add lines of text to the file specified
void appendToFile(char file_name[])
{
    FILE * file_ptr = NULL;
    char text[STRING_SIZE];
    
    // Open the file in append mode
    file_ptr = openAndCheck(file_name, "a");
    
    printf("Enter the text to add to the file\n");
    printf("Finish by entering an empty line\n");
    
    while (1)
    {
        // Ask the user for a line of text
        fgets(text, STRING_SIZE, stdin);
        // If the line has more than the enter, write it to the file
        if (strlen(text) > 1)
        {
            fprintf(file_ptr, "%s", text);
        }
        // Exit when the line is empty
        else
        {
            break;
        }
    }
    
    // Close the file
    fclose(file_ptr);
}

// Print the contents of a file to the screen
void readFile(char file_name[])
{
    FILE * file_ptr = NULL;
    char text[STRING_SIZE];
    
    // Open the file for reading
    file_ptr = openAndCheck(file_name, "r");
    
    printf("These are the contents of the file:\n");
    
    // Read until the end of the file
    while ( fgets(text, STRING_SIZE, file_ptr) )
    {
        printf("\t%s", text);
    }
    
    // Close the file
    fclose(file_ptr);
}

// Function to print a small table with square roots for numbers from 1 to 20
// This will require the use of the math library
void printSquareRoots()
{
    for (int i=1; i<=20; i++)    
    {
        printf("sqrt(%3d) = %8.2f\n", i, sqrt(i));
    }
}