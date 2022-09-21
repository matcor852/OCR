#include <stdio.h>
#include "tools.h"
#include "transformImage.h"

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
	/* resize from w * h to new_w * new_h */
	uchar **_pixels = newPixels(new_w, new_h);
	if(w*h==1)return pixels; // random
	// TODO
	return _pixels;
}
