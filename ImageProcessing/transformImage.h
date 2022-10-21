#pragma once

#include "tools.h"

void invertImage(Image *image);
void saturateImage (Image *image);
Image *resizeImage(Image *image, st new_w, st new_h);
Image* rotateImage(Image * image, int angleD);
void integrateNumber(Image *image, Image *number, Point *origin);