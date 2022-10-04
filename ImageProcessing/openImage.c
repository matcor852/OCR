#include <stdio.h>
#include <err.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "openImage.h"
#include "tools.h"

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
	SDL_PixelFormat *format = surface->format;
	Image *image = newImage(surface->w, surface->h);
	uc *pixels = image->pixels;
	st len = image->width * image->height;
	if (SDL_LockSurface(surface) != 0)
		errx(EXIT_FAILURE, "%s", SDL_GetError());
	for (st i = 0; i < len; i++)
	{
		Uint8 r, g, b;
		SDL_GetRGB(pxls[i], format, &r, &g, &b);
		Uint8 average = 0.3 * r + 0.59 * g + 0.11 * b;
		r = g = b = average;
		pixels[i] = SDL_MapRGB(format, r, g, b);
	}
	SDL_UnlockSurface(surface);
	SDL_FreeSurface(surface);
	return image;
}
