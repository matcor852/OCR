#include <stdio.h>
#include <err.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "openImage.h"

Image *openImage(const char *filename)
{
	SDL_Surface *surface_tmp = IMG_Load(filename);
	if (surface_tmp == NULL)
		errx(EXIT_FAILURE, "%s", SDL_GetError());
	SDL_Surface *surface = SDL_ConvertSurfaceFormat(surface_tmp, SDL_PIXELFORMAT_RGB888, 0);
	if (surface == NULL)
		errx(EXIT_FAILURE, "%s", SDL_GetError());
	SDL_FreeSurface(surface_tmp);
	Uint32 *pxls = surface->pixels;
	Image *image = newImage(surface->w, surface->h);
	uc *pixels = image->pixels;
	st len = image->width * image->height;
	if (SDL_LockSurface(surface) != 0)
		errx(EXIT_FAILURE, "%s", SDL_GetError());
	for (st i = 0; i < len; i++)
	{
		pixels[i] = pxls[i];
	}
	SDL_UnlockSurface(surface);
	SDL_FreeSurface(surface);
	return image;
}

void saveImage(Image *image, const char *filename)
{
    SDL_Surface *surface = imageToSurface(image);
    if (IMG_SavePNG(surface, filename) != 0)
        errx(1, "Error while saving image");
    return;
}

void saveSquare(Image *image, const char *filename, Point *point, int size)
{
    SDL_Surface *surface = imageToSurface(image);
    SDL_Rect rect = {point->x, point->y, size, size};
    SDL_Surface *cell = SDL_CreateRGBSurface(0, size, size, 32, 0, 0, 0, 0);
    SDL_BlitSurface(surface, &rect, cell, NULL);
    if (IMG_SavePNG(cell, filename) != 0)
        {errx(1, "Error while saving image");}
	SDL_FreeSurface(cell);
	SDL_FreeSurface(surface);
    return;
}