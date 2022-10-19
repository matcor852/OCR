#include <stdio.h>
#include "tools.h"
#include "transformImage.h"
#include "matrices.h"

void invertImage(Image *image)
{
	uc *pixels = image->pixels;
	st len = image->width * image->height;
	for (size_t i = 0; i < len; i++)
		pixels[i] = 255 - pixels[i];
}

void saturateImage (Image *image)
{
	uc *pixels = image->pixels;
	st w = image->width, h = image->height;
	st histo[256]= {0};

	for (st y = 0; y<h; y++)
		for (st x = 0; x<w; x++)
			histo[pixels[y*w+x]]++;

	st median = 0;
	for (st i = 255; i>=0; i--)
	{
		median += histo[i];
		if (median > w*h*0.875)
		{
			median = i;
			break;
		}
	}
	for (st y = 0; y<h; y++)
		for (st x = 0; x<w; x++)
			pixels[y*w+x] = pixels[y*w+x] >= median ? 255 : 0;
	return;
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
		float y = new_y * ration_h;
		st upper_y = y;
		st lower_y = upper_y + 1;
		for (st new_x = 0; new_x < new_w; new_x++)
		{
			float x = new_x * ratio_w;
			st left_x = x
			st right_x = left_x + 1;
			if (left_x < 0 || right_x >= w || upper_y < 0 || lower_y >= h)
			{
				new_pixels[new_y * new_w + new_x] = 0;
				continue;
			}
			float weight_left = 1 - (x - left_x);
			float weight_right = 1 - weight_left;
			float weight_top = 1 - (y - upper_y);
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

Image *extractGrid(Image *image, float corners[4][2], st new_w, st new_h)
{
	// p1 : ul, p2 : ur,
	// p3 : dl, p4 : dr
	uc *pixels = image->pixels;
	st w = image->width, h = image->height;
	Image *new_image = newImage(new_w, new_h);
	uc *new_pixels = new_image->pixels;
	float mat33[3][3];
	getTransformMatrix(corners, new_w, new_h, mat33);
	float mat31[3];
	mat31[2] = 1;
	float res[3];
	for (st new_y = 0; new_y < new_h; new_y++)
	{
		for (st new_x = 0; new_x < new_w; new_x++)
		{
			mat31[0] = new_x;
			mat31[1] = new_y;
			// mat31[2] = 1;
			matMul33_31(mat33, mat31, res);
			float x = res[0] / res[2];
			float y = res[1] / res[2];
			st left_x = x;
			st right_x = left_x + 1;
			st upper_y = y;
			st lower_y = upper_y + 1;
			if (left_x < 0 || right_x >= w || upper_y < 0 || lower_y >= h)
			{
				new_pixels[new_y * new_w + new_x] = 0;
				continue;
			}
			float weight_left = 1 - (x - left_x);
			float weight_right = 1 - weight_left;
			float weight_top = 1 - (y - upper_y);
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
