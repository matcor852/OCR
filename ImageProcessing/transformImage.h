#pragma once

#include "tools.h"

/*
uchar **resizeFloatW(uchar **pixels, st w, st h, st new_w);
uchar **resize(uchar **pixels, st w, st h, st new_w, st new_h);
*/
void invertImage(uchar *pixels, st w, st h);
uchar *resizeImage(uchar *pixels, st w, st h, st new_w, st new_h);
