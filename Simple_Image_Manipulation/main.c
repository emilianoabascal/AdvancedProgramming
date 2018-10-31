/*

Emiliano Abascal Gurria A01023234

09/03/2018

Program to make simple image manipulation

main file in which the main function is located and the program is initialized.

The program should be ran like this: './simpleImageManipulation -i 'NAME OF FILE' -o 'NAME OF RESULT' -n 'FOR THE NEGATIVE OF THE IMAGE' -s 'SCALE FACTOR'

*/

#include <stdio.h>
#include "simple_image_manipulation.h"
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char **argv) {

  /*

    Declaration of all the necessary variables needed inside the main function.

    'c' is the number that will be returned by the getopt function.

    'name' is the name of the file that will be used to manipulate.

    'oname' is the output name that the user will asign for the resulting file.

    'negFlag' is a value that will represent if the user wants to make a negative of the picture.

    'scaleFactor' is a value assigned by the user to define how big or small the image will become.

  */

  char *name = NULL;
  char *oname = NULL;
  bool negFlag = false;
  int scaleFactor = 0;
  int c;

  /*

    This loop is to check the arguments and options introduced when running thre program, inside there is a switch that analizes the value of c, and checks what are the options and arguments introduced by the user.

  */

  while ((c = getopt(argc, argv, "i:o:s:n")) != -1){
    switch (c){
      case 'i':
        name = optarg;
        break;
      case 'o':
        oname = optarg;
        break;
      case 'n':
        negFlag = true;
        break;
      case 's':
      scaleFactor = atoi(optarg);
        break;
      default:
        return 1;
        abort ();
      }//End of switch.
  }//End of loop.

/*

'picture' is a variable of type image and is assigned the value of the result of the function 'readImage' passing it the name of the input file.

*/

  image * picture = readImage(name);

  /*

if 'negFlag' is true then the picture will be the negative of itself, then if the user wants to scale it afterwars, it will call the function to scale, making the picture the resulting value of scaleImage, if the user just wants to make a negative, then the output image will be written. If the user just wants to scale it then the picture will become the scaled version of itself and then the output image will be written.

  */

  if(negFlag == true){
    picture = negativeOfImage(picture);
    if(scaleFactor == 0){
      writeImage(oname, picture);
    }else{
      picture = scaleImage(scaleFactor, picture);
      writeImage(oname, picture);
    }
  }else{
    picture = scaleImage(scaleFactor, picture);
    writeImage(oname, picture);
  }
  /*

The freeMemory Function is called passing it the image.

  */
  freeMemory(picture);
  return 0;
}
