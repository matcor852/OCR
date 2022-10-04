#include <stdio.h>
#include "tools.h"
#include "transformImage.h"

void invertImage(Image *image)
{
	uc *pixels = image->pixels;
	st len = image->width * image->height;
	for (size_t i = 0; i < len; i++)
		pixels[i] = 255 - pixels[i];
}

Image *resizeImage(Image *image, st new_w, st new_h)
{
	uc *pixels = image->pixels;
	st w = image->width, h = image->height;
	Image *new_image = newImage(new_w, new_h);
	uc *new_pixels = new_image->pixels;
	float ratio_w = (float)w / new_w;
	float ratio_h = (float)h / new_h;
	for (st new_y = 0; new_y < new_h; new_y++)
	{
		st upper_y = new_y * ratio_h;
		st lower_y = (new_y + 1) * ratio_h;
		for (st new_x = 0; new_x < new_w; new_x++)
		{
			st left_x = new_x * ratio_w;
			st right_x = (new_x + 1) * ratio_w;
			if (left_x < 0 || right_x >= w || upper_y < 0 || lower_y >= h)
			{
				new_pixels[new_y * new_w + new_x] = 0;
				continue;
			}
			float weight_left = 1 - (new_x * ratio_w - left_x);
			float weight_right = 1 - weight_left;
			float weight_top = 1 - (new_y * ratio_h - upper_y);
			float weight_bottom = 1 - weight_top;
			float value = 0;
			value += pixels[upper_y * w + left_x] * weight_left * weight_top;
			value += pixels[upper_y * w + right_x] * weight_right * weight_top;
			value += pixels[lower_y * w + left_x] * weight_left * weight_bottom;
			value += pixels[lower_y * w + right_x] * weight_right * weight_bottom;
			new_pixels[new_y * new_w + new_x] = (uc)(value + 0.5);
		}
	}
	return new_image;
}
