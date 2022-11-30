#pragma once

#include "image.h"
#include "point.h"

void saveSquare(Image *image, const char *filename, Point *point, int size);
void grayscaleImage(Image *image);
void saveImage(Image *image, const char *filename);
void tmpSaveImage(Image *image, char *destname);
void saveBoard(Image *image, const char *filename);
