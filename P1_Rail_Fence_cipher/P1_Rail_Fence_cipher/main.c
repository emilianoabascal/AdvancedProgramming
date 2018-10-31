/*

Emiliano Abascal Gurria A01023234

09/13/2018

main file in which the main function is located and the program is initialized.
The program should be ran like this: './main.o'

References:
  Enrique Lira Martinez, since forks were taught in the week i was absent, he explained to me many of the mechanics for the process communication, all the comments where '@' is, references to this reference.

*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include "P1_Rail_Fence_cipher.h"
int main() {
  
  /*

    Declaration of all the necessary variables needed inside the main function.

    'option' is the input value that will be set by the user.
    
    'filename' is the name of the file which will be assigned by the user.
    
    'fp' is the file that will be used in the program to get the .txt data.
    
    'rails' is the rails to code or decode the message/
    
    'fd' is the file Directory.
    
    'pid' is the process id.
    
  */
  
  /*
  
  While the user does not choose to quit, then the program will be running. In this loop the user is asked if it wants to encode, decode or exit, if the user wants to encode or decode, then it is asked for the file name and the number of rails. Then if it is checked if the pipe was created succesfully, then a fork is created and the child process is initiated by calling the function startWorker, then it is checked if the process has finished, if it does, then the fileDirectory is read.
  
  */
  
  while(1){
    char option;
    char filename[30];
    int rails;
    int fd[2];
    pid_t pid;
    printf("Welcome, please select an option:\nEncode(e), Decode(d), Exit(q): ");
    option = getchar();
    // Flush to remove extra cycles.
    myflush(stdin);
    switch(option){
      case 'd':
        option = 0;
          break;
      case 'e':
        option = 1;
        break;
      case 'q':
        printf("Good Bye!\n");
        return 0;
      default:
        printf("Try again\n");
          continue;
    }
    


    printf("What is the file name: ");
    scanf("%s", filename);
    printf("What is the number of rails: ");
    scanf("%d", &rails);

    // PIPE
    
    if(pipe(fd) == -1){
        printf("Error!\nQuitting!\n");
        exit(EXIT_FAILURE);
    }    
    pid = fork();

    if(pid == 0){
        startWorker(rails, filename, option, fd);//@
        exit(0);
    }else if (pid > 0){
        getPipe(fd);//@
        myflush(stdin);
    }
  }

        return 0;
}
