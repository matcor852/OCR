#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "openImage.h"
#include "tools.h"

void get1bpp(st width, st height, int pitch, uchar *pxls, uchar **pixels){
	for (st y = 0; y < height; y++)
		for (st x = 0; x < width; x++)
			pixels[y][x] = pxls[y * pitch + x];
}

void get2bpp(st width, st height, int pitch, uchar *pxls, uchar **pixels){
	for (st y = 0; y < height; y++)
		for (st x = 0; x < width; x++)
			pixels[y][x] = pxls[y * pitch + 2];
}

void get3bpp(st width, st height, int pitch, uchar *pxls, uchar **pixels){
	for (st y = 0; y < height; y++)
		for (st x = 0; x < width; x++)
			pixels[y][x] =
				(uchar)((pxls[y * pitch + x * 3] +
					 pxls[y * pitch + x * 3 + 1] +
				 	 pxls[y * pitch + x * 3 + 2]) / 3);
}

void get4bpp(st width, st height, int pitch, uchar *pxls, uchar **pixels){
	for (st y = 0; y < height; y++)
		for (st x = 0; x < width; x++)
			pixels[y][x] =
				(uchar)((pxls[y * pitch + x * 4] +
					 pxls[y * pitch + x * 4 + 1] +
				 	 pxls[y * pitch + x * 4 + 2]) / 3);
}

uchar **openImage(const char *filename, st *width, st *height){
	SDL_Surface* surface  = IMG_Load(filename);
	*width = surface->w, *height = surface->h;
	printf("width: %zu, height: %zu\n", *width, *height);
	int bpp = surface->format->BytesPerPixel;
	int pitch = surface->pitch;
	printf("pitch: %d\n", pitch);
	printf("bpp: %d\n", bpp);
	uchar *pxls = surface->pixels;
	uchar **pixels = newPixels(*width, *height);
	switch (bpp){
		case 1:
			get1bpp(*width, *height, pitch, pxls, pixels);
			break;
		case 2:
			get2bpp(*width, *height, pitch, pxls, pixels);
			break;
		case 3:
			get3bpp(*width, *height, pitch, pxls, pixels);
			break;
		case 4:
			get4bpp(*width, *height, pitch, pxls, pixels);
			break;
		default:
			printf("Error[openImage]: invalid bpp (%d)", bpp);
			exit(0);
	}
	return pixels;
}
