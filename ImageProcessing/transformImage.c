#include "transformImage.h"
#include "matrices.h"
#include "tools.h"
#include <math.h>
#include <stdio.h>

Image *copyImage(Image *image) {
	Image *copy = malloc(sizeof(Image));
	if (copy == NULL) errx(EXIT_FAILURE, "malloc failed");
	copy->width = image->width;
	copy->height = image->height;
	copy->pixels = malloc(sizeof(st) * image->width * image->height);
	if (copy->pixels == NULL) errx(EXIT_FAILURE, "malloc failed");
	for (st i = 0; i < image->width * image->height; i++)
		copy->pixels[i] = image->pixels[i];
	return copy;
}

void invertImage(Image *image) {
	uc *pixels = image->pixels;
	st len = image->width * image->height;
	for (size_t i = 0; i < len; i++) pixels[i] = 255 - pixels[i];
}

void cannyEdgeDetection(Image *image) {
	// TODO: finish this function
	// uses 5*5 gaussian kernel
	// then uses cannys edge detection algorithm

	// 1. gaussian blur
	int kernel[5][5] = {{2,  4,  5,  4, 2},
						{4,  9, 12,  9, 4},
						{5, 12, 15, 12, 5},
						{4,  9, 12,  9, 4},
						{2,  4,  5,  4, 2}};
	int kernelSum = 159;
	uc *pixels = image->pixels;
	int w = image->width;
	int h = image->height;
	uc *newPixels = malloc(sizeof(uc) * w * h);
	if (newPixels == NULL) errx(EXIT_FAILURE, "malloc failed");
	for (int y = 0; y < h; y++) {
		for (int x = 0; x < w; x++) {
			int sum = 0;
			for (int i = -2; i <= 2; i++) {
				for (int j = -2; j <= 2; j++) {
					int x2 = x + j;
					int y2 = y + i;
					if (x2 < 0 || x2 >= w || y2 < 0 || y2 >= h) continue;
					sum += pixels[y2 * w + x2] * kernel[i + 2][j + 2];
				}
			}
			newPixels[y * w + x] = sum / kernelSum;
		}
	}
}

void calibrateImage(Image *image, int radius) {
	// recalibrate the image to have a better contrast using a 10 * 10 grid
	Image *copy = copyImage(image);
	int width = image->width;
	int height = image->height;
	uc *pixels = image->pixels;
	uc *copy_pixels = copy->pixels;
	int min, max;
	for (int i = 0; i < width; i++) {
		for (int j = 0; j < height; j++) {
			min = 255;
			max = 0;
			for (int k = i - radius; k < i + radius; k++) {
				if (k < 0) {
					k = 0;
					continue;
				}
				if (k >= width) break;
				for (int l = j - radius; l < j + radius; l++) {
					if (l < 0) {
						l = 0;
						continue;
					}
					if (l >= height) break;
					if (copy_pixels[k + l * width] < min)
						min = copy_pixels[k + l * width];
					if (copy_pixels[k + l * width] > max)
						max = copy_pixels[k + l * width];
				}
			}
			if (min == max) continue;
			pixels[i + j * width] = (pixels[i + j * width] - min) * 255 / (max - min);
		}
	}
	freeImage(copy);
}

void saturateImage(Image *image) {
	uc *pixels = image->pixels;
	st w = image->width, h = image->height;
	st histo[256] = {0};

	for (st y = 0; y < h; y++)
		for (st x = 0; x < w; x++) histo[pixels[y * w + x]]++;

	st median = 0;
	for (st i = 255; i >= 0; i--) {
		median += histo[i];
		if (median > w * h * 0.875) {
			median = i;
			break;
		}
	}
	if (median == 255) median = 254;
	if (median == 0) median = 1;
	for (st y = 0; y < h; y++)
		for (st x = 0; x < w; x++)
			pixels[y * w + x] = pixels[y * w + x] >= median ? 255 : 0;
	return;
}

Image *resizeImage(Image *image, st new_w, st new_h) {
	uc *pixels = image->pixels;
	st w = image->width, h = image->height;
	Image *new_image = newImage(new_w, new_h);
	uc *new_pixels = new_image->pixels;
	float ratio_w = (float)w / new_w;
	float ratio_h = (float)h / new_h;
	for (st new_y = 0; new_y < new_h; new_y++) {
		float y = new_y * ratio_h;
		st upper_y = y;
		st lower_y = upper_y + 1;
		for (st new_x = 0; new_x < new_w; new_x++) {
			float x = new_x * ratio_w;
			st left_x = x;
			st right_x = left_x + 1;
			if (left_x < 0 || right_x >= w || upper_y < 0 || lower_y >= h) {
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
			value +=
				pixels[lower_y * w + right_x] * weight_right * weight_bottom;
			new_pixels[new_y * new_w + new_x] = (uc)(value + 0.5);
		}
	}
	return new_image;
}

Image *autoResize(Image *image, st maw_w, st max_h) {
	if (image->width <= maw_w && image->height <= max_h)
		return copyImage(image);
	// if ratio_w > ratio_h, then we resize by width
	float ratio_w = (float)image->width / maw_w;
	float ratio_h = (float)image->height / max_h;
	if (ratio_w > ratio_h)
		return resizeImage(image, maw_w, image->height / ratio_w);
	else
		return resizeImage(image, image->width / ratio_h, max_h);
}

Image *extractGrid(Image *image, Quadri *quadri, st new_w, st new_h) {
	// p1 : ul, p2 : ur,
	// p3 : dl, p4 : dr
	uc *pixels = image->pixels;
	st w = image->width, h = image->height;
	Image *new_image = newImage(new_w, new_h);
	uc *new_pixels = new_image->pixels;
	float mat33[3][3];
	getTransformMatrix(quadri, new_w, new_h, mat33);
	float mat31[3];
	mat31[2] = 1;
	float res[3];
	for (st new_y = 0; new_y < new_h; new_y++) {
		for (st new_x = 0; new_x < new_w; new_x++) {
			mat31[0] = new_x;
			mat31[1] = new_y;
			// mat31[2] = 1;
			matMul33_31(mat33, mat31, res);
			float x = res[0] / res[2];
			float y = res[1] / res[2];
			if (x < 0 || x >= w - 1 || y < 0 || y >= h - 1) {
				new_pixels[new_y * new_w + new_x] = 0;
				continue;
			}
			st left_x = x;
			st right_x = left_x + 1;
			st upper_y = y;
			st lower_y = upper_y + 1;
			float weight_left = 1 - (x - left_x);
			float weight_right = 1 - weight_left;
			float weight_top = 1 - (y - upper_y);
			float weight_bottom = 1 - weight_top;
			float value = 0;
			value += pixels[upper_y * w + left_x] * weight_left * weight_top;
			value += pixels[upper_y * w + right_x] * weight_right * weight_top;
			value += pixels[lower_y * w + left_x] * weight_left * weight_bottom;
			value +=
				pixels[lower_y * w + right_x] * weight_right * weight_bottom;
			new_pixels[new_y * new_w + new_x] = (uc)(value + 0.5);
		}
	}
	return new_image;
}

Image *rotateImage(Image *image, int angleD, uc background_color) {
	double angle = angleD * PI / 180;
	uc *pixels = image->pixels;
	st w = image->width, h = image->height;
	// all corners ((0, 0), (w, 0), (0, h), (w, h)) must be in the new image
	float _cos = cos(angle);
	float _sin = sin(angle);
	st new_w = fabs(w * cos(angle)) + fabs(h * sin(angle));
	st new_h = fabs(w * sin(angle)) + fabs(h * cos(angle));
	Image *new_image = newImage(new_w, new_h);
	uc *new_pixels = new_image->pixels;
	for (st new_y = 0; new_y < new_h; new_y++) {
		for (st new_x = 0; new_x < new_w; new_x++) {
			float x = ((float)new_x - new_w / 2) * _cos
					  - ((float)new_y - new_h / 2) * _sin + w / 2;
			float y = ((float)new_x - new_w / 2) * _sin
					  + ((float)new_y - new_h / 2) * _cos + h / 2;
			if (x < 0 || x >= w - 1 || y < 0 || y >= h - 1) {
				new_pixels[new_y * new_w + new_x] = background_color;
				continue;
			}
			st upper_y = (st)y;
			st lower_y = upper_y + 1;
			st left_x = (st)x;
			st right_x = left_x + 1;
			float weight_left = 1 - (x - left_x);
			float weight_right = 1 - weight_left;
			float weight_top = 1 - (y - upper_y);
			float weight_bottom = 1 - weight_top;
			float value = 0;
			value += pixels[upper_y * w + left_x] * weight_left * weight_top;
			value += pixels[upper_y * w + right_x] * weight_right * weight_top;
			value += pixels[lower_y * w + left_x] * weight_left * weight_bottom;
			value +=
				pixels[lower_y * w + right_x] * weight_right * weight_bottom;
			new_pixels[new_y * new_w + new_x] = (uc)(value + 0.5);
		}
	}
	return new_image;
}

// it integrate the number in the image with the origin : origin.
// only if the pixel of number is less than 10
void integrateNumber(Image *image, Image *number, Point *origin) {
	uc *pixels = image->pixels;
	st w = image->width, h = image->height;
	uc *integrated_pixels = number->pixels;
	st integrated_w = number->width, integrated_h = number->height;
	for (st y = 0; y < integrated_h; y++) {
		for (st x = 0; x < integrated_w; x++) {
			st new_x = x + origin->x;
			st new_y = y + origin->y;
			if (new_x < 0 || new_x >= w || new_y < 0 || new_y >= h
				|| integrated_pixels[y * integrated_w + x] > 20)
				continue;
			pixels[new_y * w + new_x] = integrated_pixels[y * integrated_w + x];
		}
	}
}

/*
void resize(Image *image, st new_w, st new_h) {
	Image *new_image = resizeImage(image, new_w, new_h);
	free(image->pixels);
	image->pixels = new_image->pixels;
	image->width = new_image->width;
	image->height = new_image->height;
	free(new_image);
}
*/
