/*

Emiliano Abascal Gurria A01023234

09/03/2018

File that contains all the function declarations necessary to the simple_image_manipulation.c file.

*/

#ifndef SIMPLE_IMAGE_MANIPULATION_H
#define SIMPLE_IMAGE_MANIPULATION_H

/*

Two types are defined, one which represents a pixel, containing R,G and B, and one representing the ppm image, whoch conntains a magic number, the height, width, max value and a matrix made by pixels.

*/

typedef struct p{
  unsigned short R;
  unsigned short G;
  unsigned short B;
}pixel;

typedef struct i{
  int magicNumber;
  int x;
  int y;
  int max;
  pixel ** imageMatrix;

}image;

/*

Declaration of the funtion used in the simple_image_manipulation.c file.

*/
image* readImage(char name[]);
void imageMemoryAllocator(image* picture);
image* scaleImage(double factor, image * picture);
void writeImage(char* filename, const image* img);
image* negativeOfImage(image * picture);
void freeMemory(image * picture);


#endif
