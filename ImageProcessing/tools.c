#include <stdio.h>
#include "tools.h"

uchar **newPixels(st width, st height){
	uchar **pixels = (uchar **)malloc(sizeof(uchar *) * height);
	for (st i = 0; i < height; i++)
		pixels[i] = (uchar *)malloc(sizeof(uchar) * width);
	return pixels;
}

void freeImage(uchar **pixels, st height){
	for (size_t i = 0; i < height; i++)
		free(pixels[i]);
	free(pixels);
}

void printImage(st width, st height, uchar **pixels){
	for (size_t y = 0; y < height; y++){
		for (size_t x = 0; x < width; x++)
			printf("%02x ", pixels[y][x]);
		printf("\n");
	}
}

uchar to_uchar(float n){
	return n >= (uchar)n + 0.5 ? (uchar)n + 1 : (uchar)n;
}
