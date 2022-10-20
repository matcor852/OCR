#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <err.h>
#include "openImage.h"
#include "tools.h"

void showLines(Image *background, Segment ** segments,int r, int g, int b, st nb_segments);
SDL_Surface* imageToSurface(Image *image) ;
void event_loop(SDL_Renderer* renderer, Image *image);
int rotateWithView(const char *filename);
void draw(SDL_Renderer* renderer, SDL_Texture * texture);
int displayImage(Image *image);