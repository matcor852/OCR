#pragma once

#include <stdlib.h>

typedef unsigned char uchar;
typedef size_t st;

/*
uchar **newPixels(st width, st height);
void freeImage(uchar **pixels, st height);
void printImage(st width, st height, uchar **pixels);
*/
uchar *newPixels(st width, st height);
void freeImage(uchar *pixels);
void printImage(st width, st height, uchar *pixels);
uchar to_uchar(float n);
