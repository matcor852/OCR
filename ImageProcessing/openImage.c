#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "openImage.h"
#include "tools.h"

void openImage(const char *filename, st *width, st *height, uchar **pixels){
	printf("Opening file '%s'\n", filename);
	SDL_Surface* surface  = IMG_Load(filename);
	*width = surface->w, *height = surface->h;
	printf("width: %zu, height: %zu\n", *width, *height);
	int bpp = surface->format->BytesPerPixel;
	int pitch = surface->pitch;
	printf("pitch: %d\n", pitch);
	printf("bpp: %d\n", bpp);
	uchar *pxls = surface->pixels;
	pixels = (uchar **)malloc(sizeof(uchar *) * *height);
	for (size_t i = 0; i < *height; i++)
		pixels[i] = (uchar *)malloc(sizeof(uchar) * *width);
	unsigned short value;
	for (size_t y = 0; y < *height; y++){
		for (size_t x = 0; x < *width; x++){
			value = 0;
			uchar *pxl = pxls + (y * pitch + x * bpp);
			switch (bpp){
				case 1:
				case 2:
					value = *pxl;
					break;
				case 3:
				case 4:
					value += pxl[0];
					value += pxl[1];
					value += pxl[2];
					value /= 3;
					break;
				default:
					break;
			}
			pixels[y][x] = value;
		}
	}
	for (size_t y = 0; y < *height; y++){
		for (size_t x = 0; x < *width; x++)
			printf("%02x ", pixels[y][x]);
		printf("\n");
	}
}
