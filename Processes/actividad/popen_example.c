#include<stdio.h>
#include <stdlib.h>
#define LENGTH 10

int main(){
  //char * command = "date +%H:%M";
  char * command = "grep 55";
  char response[LENGTH];
  FILE * output = popen(command, "w");
  if(!output){
    perror("no siabrio");
    exit(EXIT_FAILURE);
  }

  // fgets(response, LENGTH, output);
  // printf("The time i got is %s\n", response);

  fprintf(output, "1234567890\n");
  fprintf(output, "55813491234-102348\n");
  pclose(output);
}
