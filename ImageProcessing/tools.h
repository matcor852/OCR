#pragma once

#include <stdlib.h>

#define FILENAME "Images/image_04.jpeg"

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
	st x, y;
} Point;

typedef struct
{
	st x1, y1, x2, y2, theta, r, length;
} Segment;

typedef struct
{
	Point *p1, *p2, *p3, *p4;
} Square;

Image *newImage(st width, st height);
Point *newPoint(st x, st y);
Square *newSquare(Point *p1, Point *p2, Point *p3, Point *p4);
void freeImage(Image *image);
void printImage(Image *image);
