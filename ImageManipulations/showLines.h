#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <err.h>
#include "tools.h"

typedef struct
{
	st x1, y1, x2, y2, theta, length;
} Segment;

void showLines(const char * filename, Segment ** segments);