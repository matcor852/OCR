#include <stdio.h>
#include "tools.h"

Image *newImage(st width, st height)
{
	Image *image = malloc(sizeof(Image));
	image->pixels = malloc(width * height * sizeof(uc));
	image->width = width;
	image->height = height;
	return image;
}

Point *newPoint(st x, st y)
{
	Point *point = malloc(sizeof(Point));
	point->x = x;
	point->y = y;
	return point;
}

Square *newSquare(Point *p1, Point *p2, Point *p3, Point *p4)
{
	Square *square = malloc(sizeof(Square));
	square->p1 = p1;
	square->p2 = p2;
	square->p3 = p3;
	square->p4 = p4;
	return square;
}

void freeImage(Image *image)
{
	free(image->pixels);
	free(image);
}

void printImage(Image *image)
{
	uc *pixels = image->pixels;
	st w = image->width, h = image->height;
	for (size_t y = 0; y < h; y++)
	{
		for (size_t x = 0; x < w; x++)
			printf("%02x ", pixels[y * w + x]);
		printf("\n");
	}
}
