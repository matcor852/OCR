#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <err.h>
#include "tools.h"

void showLines(const char *filename, Segment **segments, int r, int g, int b, st nb_segments);
