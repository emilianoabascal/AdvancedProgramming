/*
Show the size of the data that can be stored in different variable types

Emiliano Abascal Gurria A01023234
*/

#include <stdio.h>
#include <limits.h>
#define NUMBER 15

int main(){
  printf("hello %d\n", NUMBER);
  #ifdef DEBUG
  printf("The temp value is");
  #endif

  printf("INT MIN: %d - MAX: %d\n",INT_MIN, INT_MAX);
  printf("LONG MIN: %ld - MAX: %ld\n",LONG_MIN, LONG_MAX);
  return 0;
}
