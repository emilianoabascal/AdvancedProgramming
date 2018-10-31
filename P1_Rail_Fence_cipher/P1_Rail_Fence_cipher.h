
/*

Emiliano Abascal Gurria A01023234

09/13/2018

File that contains all the function declarations necessary to the P1_Rail_Fence_cipher.c file.

References:
  Enrique Lira Martinez, since forks were taught in the week i was absent, he explained to me many of the mechanics for the process communication, all the comments where '@' is, references to this reference.

*/

#ifndef P1_Rail_Fence_cipher_H
#define P1_Rail_Fence_cipher_H
#include <stdio.h>
#define BUFFER_SIZE 512

void myflush(FILE *in);
void startWorker(int rails, char* fileName, char action, int* fd);
char * encode(char * text, int rails);
char * decode(char * text, int rails);
void getPipe(int fd[]);
#endif
