#include <stdio.h>
#include <math.h>
#include "tools.h"
#include "openImage.h"
#define PI 3.141592654

void test(void){
	/*
	float _r = 24, _theta = 69;
	float _cos = cos(_theta * PI / 180), _sin = sin(_theta * PI / 180);
	st width = 50, height = 30;
	uchar **pixels = newPixels(width, height);
	for (st y = 0, x = 0; y < height; x++, y += x / width, x %= width){
		if (fabs(_r - x * _cos - y * _sin) < 1)
			pixels[y][x] = 255;
		else
			pixels[y][x] = 0;
	}
	printImage(width, height, pixels);
	*/
	st width, height;
	uchar **pixels = openImage("Images/image_04.jpeg", &width, &height);
	st r_max = sqrt(width * width + height * height);
	int **r_theta = (int **)malloc(sizeof(int *) * r_max);
	for (st r = 0; r < r_max; r++)
		r_theta[r] = (int *)malloc(sizeof(int) * 360);
	int total;
	int nb_pixels;
	int best_r, best_theta;
	int best_value = 0;
	for (st r = 0; r < 100; r++){//r_max
		for (st theta = 0; theta < 360; theta++){
			total = 0;
			nb_pixels = 0;
			for (st y = 0; y < 400; y++){//height
				for (st x = 0; x < 400; x++){//width
					if (fabs(r - x * cos(theta * PI / 180) - y * sin(theta * PI / 180)) < 1){
						total += 255 - pixels[y][x];
						nb_pixels++;
					}
				}
			}
			r_theta[r][theta] = nb_pixels ? total / sqrt(nb_pixels) : 0;
			if (r_theta[r][theta] > best_value){
				best_r = r;
				best_theta = theta;
				best_value = r_theta[r][theta];
			}
		}
	}
	for (st r = 0; r < 40; r++){
		for (st theta = 0; theta < 360; theta += 2){
			printf("%02x", (int)(255 * ((float)r_theta[r][theta] / best_value)));
		}
		printf("\n");
	}
	printf("r = %d, theta = %d, value = %d\n", best_r, best_theta, best_value);
}
