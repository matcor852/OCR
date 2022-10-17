#pragma once

#include <stdlib.h>

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
	st x1, y1, x2, y2, theta, length;
} Segment;

typedef struct
{
    st x1, y1;
    st x2, y2;
    st x3, y3;
    st x4, y4;
} Square;

Image *newImage(st width, st height);
void freeImage(Image *image);
void printImage(Image *image);
