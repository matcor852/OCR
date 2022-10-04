#pragma once

#include <stdlib.h>

typedef unsigned char uc;
typedef size_t st;

struct {
    uc *pixels;
    st width;
    st height;
} typedef Image;

Image *newImage(st width, st height);
void freeImage(Image *image);
void printImage(Image *image);
