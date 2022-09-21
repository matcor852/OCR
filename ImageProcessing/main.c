#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "tools.h"
#include "openImage.h"
#include "transformImage.h"

int main(){
	/*
	char *filename = "Images/image001.png";
	st width, height;
	uchar **pixels = openImage(filename, &width, &height);
	*/
	uchar **pixels = newPixels(5, 1);
	pixels[0][0] = 0;
	pixels[0][1] = 3;
	pixels[0][2] = 2;
	pixels[0][3] = 6;
	pixels[0][4] = 4;
	st width = 5, height = 1;
	printImage(width, height, pixels);
	uchar **resized = resizeFloatW(pixels, width, height, 3);
	width = 3;
	freeImage(pixels, height);
	printImage(width, height, resized);
	return 0;
}
