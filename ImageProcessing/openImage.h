#pragma once
#include "tools.h"
#include "transformImage.h"
#include "display.h"

Image *openImage(const char *filename);
void saveSquare(Image *image, const char *filename, st x, st y, st size);
void saveImage(Image *image, const char *filename);