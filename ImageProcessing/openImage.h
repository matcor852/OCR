#pragma once
#include "tools.h"

/*
void get1bpp(st width, st height, int pitch, uchar *pxls, uchar **pixels);
void get2bpp(st width, st height, int pitch, uchar *pxls, uchar **pixels);
void get3bpp(st width, st height, int pitch, uchar *pxls, uchar **pixels);
void get4bpp(st width, st height, int pitch, uchar *pxls, uchar **pixels);
uchar **openImage(const char *filename, st *width, st *height);
*/
uchar *openImage(const char *filename, st *width, st *height);
