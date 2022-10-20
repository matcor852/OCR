#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <err.h>
#include "tools.h"

void showLines(const char *filename, Segment **segments, int r, int g, int b, st nb_segments);
Image* rotateImage(Image * image, double angle) ;
SDL_Surface* imageToSurface(Image *image) ;
void event_loop(SDL_Renderer* renderer, SDL_Texture *texture);
void draw(SDL_Renderer* renderer, SDL_Texture * texture);
int displayImage(Image *image);