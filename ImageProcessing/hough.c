#include <stdio.h>
#include <math.h>
#include "tools.h"
#include "openImage.h"
#define PI 3.141592654

void getY(st x, st *y, st r, float _cos, float _sin, st height){
	float value = (r - x * _cos) / _sin;
	*y = 0 <= value && value < height ? value : 0;
}

void getX(st *x, st y, st r, float _cos, float _sin, st width){
	float value = (r - y * _sin) / _cos;
	*x = 0 <= value && value < width ? value : 0;
}

void test(void){
	st width, height;
	uchar **pixels = openImage("Images/image_04.jpeg", &width, &height);
	for (st y = 0; y < height; y++)
		for (st x = 0; x < width; x++)
			pixels[y][x] = 255 - pixels[y][x];
	st r_max = sqrt(width * width + height * height);

	int **r_theta = (int **)malloc(sizeof(int *) * r_max);
	for (st r = 0; r < r_max; r++)
		r_theta[r] = (int *)malloc(sizeof(int) * 360);
	int total;
	int nb_pixels;
	st best_r, best_theta;
	int best_value = 0;
	for (st r = 0; r < r_max; r++){
		for (st theta = 0; theta < 360; theta++){
			if (180 <= theta && theta <= 270){
				r_theta[r][theta] = 0;
				continue;
			}
			st x_min = width, x_max = 0, y_max = 0, y_min = height;
			float _sin = sin(theta * PI / 180);
			float _cos = cos(theta * PI / 180);
			total = 0;
			nb_pixels = 0;
			if ((45 <= theta && theta <= 135) || (225 <= theta && theta <= 315)){
				for (st x = 0; x < width; x++){
					st y;
					getY(x, &y, r, _cos, _sin, height);
					if (fabs(r - x * _cos - y * _sin) < 1){
						if (x > x_max)
							x_max = x;
						if (x < x_min)
							x_min = x;
						if (y > y_max)
							y_max = y;
						if (y < y_min)
							y_min = y;
						total += pixels[y][x];
						nb_pixels++;
					}
				}
			}
			else {
				for (st y = 0; y < height; y++){
					st x;
					getX(&x, y, r, _cos, _sin, width);
					if (fabs(r - x * _cos - y * _sin) < 1){
						if (x > x_max)
							x_max = x;
						if (x < x_min)
							x_min = x;
						if (y > y_max)
							y_max = y;
						if (y < y_min)
							y_min = y;
						total += pixels[y][x];
						nb_pixels++;
					}
				}
			}
			uchar value = (nb_pixels && (x_max - x_min > width / 4 || y_max - y_min > height / 4)) ? total / nb_pixels : 0;
			r_theta[r][theta] = value;
			if (value > best_value){
				best_r = r;
				best_theta = theta;
				best_value = value;
			}
			if (!value)
				theta++;
		}
	}
	for (st r = 0; r < r_max; r += 2){
		for (st theta = 0; theta < 360; theta += 2){
			printf("%02x", (int)(255 * ((float)r_theta[r][theta] / best_value)));
		}
		printf("\n");
	}
	printf("r = %zu, theta = %zu, value = %d\n", best_r, best_theta, best_value);
	int p1x = 0, p1y = best_r / sin(best_theta * PI / 180);
	int p2y = 0, p2x = best_r / cos(best_theta * PI / 180);
	int p3x = width - 1, p3y = (best_r - p3x * cos(best_theta * PI / 180)) / sin(best_theta * PI / 180);
	int p4y = 0, p4x = (best_r - p4y * sin(best_theta * PI / 180)) / cos(best_theta * PI / 180);
	printf("p1 = (%d, %d); p2 = (%d, %d)\n", p1x, p1y, p2x, p2y);
	printf("p3 = (%d, %d); p4 = (%d, %d)\n", p3x, p3y, p4x, p4y);
}
