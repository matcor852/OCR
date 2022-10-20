#pragma once

#include <stdlib.h>

#define FILENAME "Images/image_04.jpeg"
#define PI 3.141592654

typedef unsigned char uc;
typedef size_t st;

typedef  struct
{
    uc *pixels;
    st width;
    st height;
} Image;

typedef struct
{
	st x1, y1, x2, y2, theta, r, length;
} Segment;

typedef struct
{
    Segment *s1,*s2,*s3,*s4;
} Square;

Image *newImage(st width, st height);
void freeImage(Image *image);
void printImage(Image *image);
