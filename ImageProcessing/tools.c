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
