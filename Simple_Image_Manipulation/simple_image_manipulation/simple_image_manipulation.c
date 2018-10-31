/*

Emiliano Abascal Gurria A01023234

09/03/2018

File that contains all the functios necessary to manipulate images.

*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "simple_image_manipulation.h"

/*a

Function to read the ppm image and assign it to an image type variable, it recieves the name of the file to be read.

*/

image* readImage(char name[]){

/*

  Declaration of all the necessary variables needed inside the function.

  'f' is the file that is going to be read.

  'picture' is a variable of type image that will store all the information of the ppm image that will be read.

  'buffer' is a string in which every line of the file will be temporarely stored.

*/

  FILE *f = fopen(name, "r");
  image *picture;
  char buffer[3];

  /*

  Memory space is assigned to picture, then the first line of the file is stored in the buffer, then the value in the second space will be stored in 'magicNumber' which is a value inside of picture.

  Then in the next line, the values of the width and height of the matrix(picture) are stored, in the next line the max value is stored and the function imageMemoryAllocator is called passing it the image.

  */

  picture = (image*)malloc(sizeof(image));
  fgets(buffer, 3, f);
  picture -> magicNumber = atoi(&buffer[1]);
  fscanf(f, "%i %i\n%i", &picture -> y, &picture -> x, &picture -> max);
  imageMemoryAllocator(picture);

/*

If the magic number inside the file is P6, then there are two loops, one that goes from zero to the height of the matrix and one that goes from zero two the width of the matrix, then the imageMatrix inside of picture (which is a two dimentional matrix of a structure which contains the values R,G,B) is assigned the value of each pixel.

If the magic number is not p6, then the cannot be worked with.

*/

  if(picture -> magicNumber == 6){
    for(int i = 0; i < picture -> y; i++){
      for(int j = 0; j < picture -> x; j++){
        picture -> imageMatrix[i][j].B = fgetc(f);
        picture -> imageMatrix[i][j].R = fgetc(f);
        picture -> imageMatrix[i][j].G = fgetc(f);
      }//End of loop.
    }//End of loop.
  }else{
    printf("This is not a bianary image.");
  }
  fclose(f);//The file is closed.
  return picture;//The picture is returned.
}

/*

Function to assign memory space to the image, it receives an image type.

*/


void imageMemoryAllocator(image* picture){
/*

Space in memory is assigned for the imageMatrix inside of picture, then space in memory is assigned for all the rows and columns inside imageMatrix.

*/

  picture -> imageMatrix = (pixel**) malloc ((picture-> y)*sizeof(pixel*));
  picture -> imageMatrix[0] = (pixel*) calloc (picture-> x * picture-> y,sizeof(pixel*));
  for(int i = 0; i < picture -> y; i++){
    picture -> imageMatrix[i] = picture -> imageMatrix[0] + picture->x * i;
  }
}

/*

Function to scale an image, it receives the factor of which the image will be scaled to and the picture, it returns an image.

*/

image* scaleImage(double factor, image * picture){

  /*

    Declaration of all the necessary variables needed inside the function.

    'picture2' is a picture of type image that will become the scaled image.

  */

  image *picture2;

/*

Factor is equal to the factor divided by a hundred, then memory is assigned to the new picture, then the size of the image is multiplied by the scale factor, then the magicNumber on picture2 is the same as in picture, then memory is allocated for the ImageMatrix inside of picture2, by calling imageMemoryAllocator.

*/

  factor = factor/100;

  picture2 = (image*)malloc(sizeof(image));
  picture2 -> x = picture ->x*factor;
  picture2 -> y = picture ->y*factor;
  picture2 -> magicNumber = picture ->magicNumber;
  picture2 -> max = picture ->max;
  imageMemoryAllocator(picture2);

/*

Then there are two loops, one that goes from zero to the height of picture2, and another that goes from zero to the width of picture2, then the imageMatrix in the position i,j is equal to the imageMatrix in the original picture at the position of the floor operation[i/factor] and the floor operation[j/factor].

*/


    for (int i = 0; i < picture2 -> y; i++) {
        for (int j = 0; j < picture2 -> x; j++) {
          if(factor < 1){
            picture2-> imageMatrix[i][j] = picture -> imageMatrix[(int)(floor(i/factor))][(int)(floor(j/factor))];
          }
        }
    }
    return picture2;//The scaled image is returned.
}

/*

Function to make the negative of an image, it receives an image type, and returns an image.

*/

image* negativeOfImage(image * picture){

  /*

    Declaration of all the necessary variables needed inside the function.

    'picture2' is a picture of type image that will become the negative image.

  */

  image *picture2;

  /*

  The memory is assigned to the new picture, then the size of the image is multiplied by the scale factor, then the magicNumber on picture2 is the same as in picture, then memory is allocated for the ImageMatrix inside of picture2, by calling imageMemoryAllocator.

  */

  picture2 = (image*)malloc(sizeof(image));
  picture2 -> x = picture ->x;
  picture2 -> y = picture ->y;
  picture2 -> magicNumber = picture ->magicNumber;
  picture2 -> max = picture ->max;
  imageMemoryAllocator(picture2);


/*

Then there are two loops, one that goes from zero to the height of picture2, and another that goes from zero to the width of picture2, then the imageMatrix in the position i,j is equal to the substraction of the full color of R, G and B and the value of the respective color of each pixel.

*/

    for (int i = 0;i < picture2 -> y; i++) {
        for (int j = 0;j < picture2 -> x; j++) {
            picture2-> imageMatrix[i][j].R = 255 - picture -> imageMatrix[i][j].R;
            picture2-> imageMatrix[i][j].G = 255 - picture -> imageMatrix[i][j].G;
            picture2-> imageMatrix[i][j].B = 255 - picture -> imageMatrix[i][j].B;
        }//End of loop
    }//End of loop
    return picture2;//The negative image is returned.
}

/*

Function to free memory space of images, it receives a picture.

*/

void freeMemory(image * picture){
   free(picture -> imageMatrix[0]);
   free(picture -> imageMatrix);
   free(picture);
}

/*

Function to write the modified image into a new image.

*/

void writeImage(char* filename, const image* picture){

  /*

    Declaration of all the necessary variables needed inside the function.

    'file' is the file to be created

  */

  FILE* file = fopen(filename, "w");

/*

The magic number, the height and width of the image, as well as the picture max, is printed on the first lines of the file.

Then there are two loops, one that goes from zero to the height of picture, and another that goes from zero to the width of picture, in which the bianary values of RGB of each pixel are printed on the file.

*/

  fprintf(file, "P6\n%i %i\n%i\n", picture->y, picture->x, picture->max);
    for(int i = 0; i < picture -> y; i++){
      for(int j = 0; j < picture -> x; j++){
        fwrite(&picture->imageMatrix[i][j].R, 1, 1, file);
        fwrite(&picture->imageMatrix[i][j].G, 1, 1, file);
        fwrite(&picture->imageMatrix[i][j].B, 1, 1, file);
      }//End of loop
  }//End of loop
  fclose(file);//The File is closed
}
