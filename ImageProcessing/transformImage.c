#include <stdio.h>
#include "tools.h"
#include "transformImage.h"

/*
uchar **resizeFloatW(uchar **pixels, st w, st h, st new_w){
	uchar **new_pixels = newPixels(new_w, h);
	float ratio = (float)w / new_w;
	for (size_t j = 0; j < h; j++){
		size_t i = 0;
		size_t new_i = 0;
		float value = 0;
		float weight;
		float total_weight = 0;
		while (i < w){
			if ((new_i + 1) * ratio >= i + 1){
				if (new_i * ratio > i)
					weight = i + 1 - new_i * ratio;
				else
					weight = 1;
				value += pixels[j][i] * weight;
				total_weight += weight;
				i++;
			}
			else if ((new_i + 1) * ratio >= i){
				weight = (new_i + 1) * ratio - i;
				value += pixels[j][i] * weight;
				total_weight += weight;
				printf("%f ", value / total_weight);
				printf("%d ", to_uchar(value / total_weight));
				new_pixels[j][new_i] = (uchar)(value / total_weight);
				new_i++;
				value = 0;
				total_weight = 0;
			}
			else {
				printf("%f ", value / total_weight);
				printf("%d ", to_uchar(value / total_weight));
				new_pixels[j][new_i] = (uchar)(value / total_weight);
				new_i++;
				value = 0;
				total_weight = 0;
			}
		}
		new_pixels[j][new_i] = (uchar)(value / total_weight);
	}
	return new_pixels;
}

uchar **resize(uchar **pixels, st w, st h, st new_w, st new_h){
	// resize from w * h to new_w * new_h
	uchar **_pixels = newPixels(new_w, new_h);
	if(w*h==1)return pixels; // random
	// TODO
	return _pixels;
}
*/

void invertImage(uchar *pixels, st w, st h)
{
	for (size_t i = 0; i < w * h; i++)
		pixels[i] = 255 - pixels[i];
}

uchar *resizeImage(uchar *pixels, st w, st h, st new_w, st new_h)
{
	uchar *new_pixels = newPixels(new_w, new_h);
	float ratio_w = (float)w / new_w;
	float ratio_h = (float)h / new_h;
	float x, y;
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
			float weight_left = 1 - (x - (int)x);
			float weight_right = 1 - weight_left;
			float weight_top = 1 - (y - (int)y);
			float weight_bottom = 1 - weight_top;
			float value = 0;
			value += pixels[upper_y * w + left_x] * weight_left * weight_top;
			value += pixels[upper_y * w + right_x] * weight_right * weight_top;
			value += pixels[lower_y * w + left_x] * weight_left * weight_bottom;
			value += pixels[lower_y * w + right_x] * weight_right * weight_bottom;
			new_pixels[new_y * new_w + new_x] = to_uchar(value);
		}
	}
	return new_pixels;
}
