#include <stdio.h>
#include <err.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "openImage.h"
#include "tools.h"

/*
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
*/

uchar *openImage(const char *filename, st *width, st *height)
{
	SDL_Surface *surface_tmp = IMG_Load(filename);
	if (surface_tmp == NULL)
		errx(EXIT_FAILURE, "%s", SDL_GetError());
	SDL_Surface *surface = SDL_ConvertSurfaceFormat(surface_tmp, SDL_PIXELFORMAT_RGB888, 0);
	if (surface == NULL)
		errx(EXIT_FAILURE, "%s", SDL_GetError());
	SDL_FreeSurface(surface_tmp);
	*width = surface->w, *height = surface->h;
	uchar *pixels = newPixels(*width, *height);
	int len = surface->w * surface->h;
	Uint32 *pxls = surface->pixels;
	SDL_PixelFormat *format = surface->format;
	if (SDL_LockSurface(surface) != 0)
		errx(EXIT_FAILURE, "%s", SDL_GetError());
	for (int i = 0; i < len; i++)
	{
		Uint8 r, g, b;
		SDL_GetRGB(pxls[i], format, &r, &g, &b);
		Uint8 average = 0.3 * r + 0.59 * g + 0.11 * b;
		r = g = b = average;
		pixels[i] = SDL_MapRGB(format, r, g, b);
	}
	SDL_UnlockSurface(surface);
	SDL_FreeSurface(surface);
	return pixels;
}
