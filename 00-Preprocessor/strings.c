/*
Experiments with strings

Emiliano Abascal Gurria A01023234

*/

#include <stdio.h>
#include <string.h>
#include "stringFuncs.h"
#define string_size 20



int main(){
  char name[string_size] = "hi";
  printf("%s\n", name);
  strcpy(name, "wazup");
  strncpy(name, "wazup", string_size);
  //readString(name, string_size);

  printf("%s\n", name);

  //scanf("%s", name);
  //printf("%s\n", name);


  return 0;
}


