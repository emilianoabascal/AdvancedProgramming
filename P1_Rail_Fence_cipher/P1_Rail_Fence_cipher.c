/*

Emiliano Abascal Gurria A01023234

09/13/2018

File that contains all the functios necessary to encode or decode a message.

References:
  Enrique Lira Martinez, since forks were taught in the week i was absent, he explained to me many of the mechanics for the process communication, all the comments where '@' is, references to this reference.

*/
#include "P1_Rail_Fence_cipher.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

//Function to delete user input in the main menu.
void myflush(FILE *in){
  int ch;
  do
      ch = fgetc ( in );
  while ( ch != EOF && ch != '\n' );
  clearerr ( in );
}
/*

Function to read the file and encode or decode the message in the .txt file, it receives the rail number, the file name, the action to be done and the fileDirectory.

*/
void startWorker(int rails, char* fileName, char action, int* fd){
  
  /*

    Declaration of all the necessary variables needed inside the function.

  */
  
  FILE * fp;
  char * line = NULL;
  size_t len = 0;
  ssize_t read;
  int outputFileNameSize = strlen(fileName);
  
/* 

File Descriptor for reading is closed, then the file with the pipe descriptor is opened so the communication between processes can be done, then, then the name of the resulting file is created and opened.(@)

*/
  close(fd[0]);
  FILE* pFile = fdopen(fd[1], "w");
  
  char* outName = malloc((outputFileNameSize + 8 + 1) * sizeof(char));
  strcpy(outName, "result_");
  strcat(outName, fileName);
  char* outfileName = outName;
  FILE* out = fopen(outfileName, "w");


/* 

  The original .txt file and then there is a loop that will run until the last line is read inside the file, if the last character of the line is '\n' then it is replaced to avoid problems in the encode or decode functions, then if the action is to encode or decode, the result of the function call is printed in the resulting file.

*/

  fp = fopen(fileName, "r");  
  while ((read = getline(&line, &len, fp)) != -1) {
    if(line[strlen(line)-1] == '\n'){
      line[strlen(line)-1] = '\0';
    }
    if(action){
      fprintf(out, "%s\n", encode(line, rails));
    }else{
      fprintf(out, "%s\n", decode(line, rails));
    }
  }//End of loop
  
  /* 

    The read and write files are closed and the variables outName and line are removed from memory, then in the process is sent to the main function and the name of the resulting file is removed from memory, the pFile is closed and the fileDirectory on position 1 as well.

  */

  
  fclose(fp);
  fclose(out);
  free(outName);
  free(line);
  fprintf(pFile, "%s", outfileName);
  fclose(pFile);
  close(fd[1]);
}

/*

Function to encode a string, it receives the string and the number of rails.

*/
char * encode(char * text, int rails){
  /*

    Declaration of all the necessary variables needed inside the function, and memory allocation for the matrix and the resulting message that will be generated in the program.

  */
  char ** encodedMatrix;
  char * encodedMessage;
  int tempIndex = 0;
  int auxVar = -1;
  int y = 0;
  int x = 0;
  
  encodedMatrix = calloc(strlen(text), sizeof(char*));
  encodedMessage = calloc(strlen(text), sizeof(char));
  for(int i = 0; i < rails; i++) {
    encodedMatrix[i] = calloc(strlen(text), sizeof(char));
    for(int j = 0; j < strlen(text); ++j){
      encodedMatrix[i][j] = '\n';
    }//End of loop
  }//End of loop
  
  /*
  
  Loop to iterate between spaces inside the matrix assigning the value in the space y,x, using auxVar which will be multiplying itself with -1 to make itself a negative and positive index in each run, only if the row is 0 or the number of rails -1, then the row is equal to itself plus auxVar.
  
  */


  for(int i = 0; i < strlen(text); ++i){
    encodedMatrix[y][x++] = text[i];
    if(y == 0 || y == rails-1){
      auxVar = auxVar * (-1);
    }
    y = y + auxVar;
  }//End of loop
  
  /*
  
  Loop to assign the characters assigned in the matrix to the message, making it an encrypted string, if the matrix in the spaces of both iterators is not '\n' then the encoded message will be equal to the encodedMatrix in the spaces of the iterators and the tempIndex will be itself plus one.
  
  */

  for(int i = 0; i < rails; ++i){
      for(int j = 0; j < strlen(text); ++j){
        if(encodedMatrix[i][j] != '\n'){
          encodedMessage[tempIndex] = encodedMatrix[i][j];
          tempIndex++;
        }
      }//End of loop
    }//End of loop
    
  /*
  
  Memory inside of encodedMatrix is removed from memory and the encoded string is returned.
  
  */
    
  for(int i = 0; i <= rails; i++) {
    free(encodedMatrix[i]);
  }//End of loop
  free(encodedMatrix);
  return encodedMessage;
}

/*

Function to decode a string, it receives the string and the number of rails.

*/
char * decode(char * text, int rails){
  /*

    Declaration of all the necessary variables needed inside the function, and memory allocation for the matrix and the resulting message that will be generated in the program.

  */
  char ** encodedMatrix;
  char * decodedMessage;
  int auxVar = -1;
  int y = 0;
  int x = 0;
  int auxIndex = 0;
  int tempIndex = 0;

  encodedMatrix = calloc(strlen(text), sizeof(char*));
  decodedMessage = calloc(strlen(text), sizeof(char));

  for(int i = 0; i < rails; ++i) {
    encodedMatrix[i] = calloc(strlen(text), sizeof(char));
    for(int j = 0; j < strlen(text); ++j){
      encodedMatrix[i][j] = '\n';
    }//End of loop
  }//End of loop

  /*
  
  Loop to iterate between spaces inside the matrix assigning the value in the space y,x so that in another loop the program knows where the values of the original text will be placed, using auxVar which will be multiplying itself with -1 to make itself a negative and positive index in each run, only if the row is 0 or the number of rails -1, then the row is equal to itself plus auxVar.
  
  */

  for(int i = 0; i < strlen(text); ++i){
    encodedMatrix[y][x++] = '*';
    if(y == 0 || y == rails-1){
        auxVar = auxVar * (-1);
    }//End of loop
    y = y + auxVar;
  }//End of loop

  /*
  
  Loop to assign to the matrix the values of the original text, if the encodedMatrix in the index of both iterators is equal to '*' then the value of the matrix in this index will be the char in the index of the auxIterator inside the original text.
  
  */

  for(int i = 0; i < rails; ++i){
    for(int j = 0; j < strlen(text); ++j){
      if(encodedMatrix[i][j] == '*'){
          encodedMatrix[i][j] = text[auxIndex];
          auxIndex++;
      }
    }//End of loop
  }//End of loop

  y = 0;
  x = 0;
  auxVar = -1;
/*

This loop is used to assign the values of the encoded Matrix to the decoded message.

*/
  for(int i = 0; i < strlen(text); ++i){
    decodedMessage[tempIndex] = encodedMatrix[y][x++];
    tempIndex++;
    if(y == 0 || y == rails-1){
      auxVar = auxVar * (-1);
    }
    y = y + auxVar;
  }//End of loop
  
  /*
  
  Memory inside of encodedMatrix is removed from memory and the decoded string is returned.
  
  */
  
  for(int i = 0; i <= rails; i++) {
    free(encodedMatrix[i]);
  }//End of loop
  free(encodedMatrix);
  
  return decodedMessage;
}

/*

Function to get the pipe to make the process communication, it receives a file directory.

*/

void getPipe(int fd[]){
  FILE * fp = NULL;
  char fname[BUFFER_SIZE];
  /*
  
  Close the descriptor for writing then the file descriptor is opened so it can get the data from the stream in the loop and the pointer is closed as well as the file descriptor.(@)
  
  */
  close(fd[1]);
  fp = fdopen(fd[0], "r");
  while (fgets(fname, BUFFER_SIZE, fp)){
      printf("\n\nThe resulting file was saved in %s\n\n", fname);
  }
  fclose(fp);
  close(fd[0]);
}

