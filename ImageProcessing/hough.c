#include <stdio.h>
#include <math.h>
#include "tools.h"
#include "openImage.h"
#include "transformImage.h"
#define PI 3.141592654

void getY(st x, st *_y, st *y_, st r, float _cos, float _sin)
{
	float value = (r - x * _cos) / _sin;
	*_y = value - 2;
	*y_ = value + 2;
}

void getX(st *_x, st *x_, st y, st r, float _cos, float _sin)
{
	float value = (r - y * _sin) / _cos;
	*_x = value - 2;
	*x_ = value + 2;
}

int XtoY(st x, st r, float _cos, float _sin)
{
	return (r - x * _cos) / _sin;
}

int YtoX(st y, st r, float _cos, float _sin)
{
	return (r - y * _sin) / _cos;
}

uc *getLine(uc **pixels, st width, st height, int r, int theta, int threshold, st *len)
{
	float _sin = sin(theta * PI / 180);
	float _cos = cos(theta * PI / 180);
	if ((45 <= theta && theta <= 135) || (225 <= theta && theta <= 315))
	{
		uc *line = (uc *)malloc(width * sizeof(uc));
		for (st x = 0; x < width; x++)
		{
			st y = XtoY(x, r, _cos, _sin);
			if (y < 0 || y >= height)
			{
				line[x] = 0;
				continue;
			}
			line[x] = pixels[y][x] > threshold ? 255 : 0;
		}
		*len = width;
		return line;
	}
	uc *line = malloc(height * sizeof(uc));
	for (st y = 0; y < height; y++)
	{
		st x = YtoX(y, r, _cos, _sin);
		if (x < 0 || x >= width)
		{
			line[y] = 0;
			continue;
		}
		line[y] = pixels[y][x] > threshold ? 255 : 0;
	}
	*len = height;
	return line;
}

void smoothLine(uc *line, st len)
{
	uc histo[256] = {0};
	for (st i = 0; i < len; i++)
		histo[line[i]]++;
	unsigned int total = 0;
	uc threshold = 0;
	while (total < len / 2)
		total += histo[threshold++];
	for (st i = 0; i < len; i++)
		line[i] = line[i] > threshold ? 1 : 0;
	for (st r_smooth = 1; r_smooth < len / 2; r_smooth++)
	{
		uc newLine[len];
		for (st i = 0; i < len; i++)
		{
			st nb = 0;
			st sum = 0;
			for (st j = i - r_smooth; j <= i + r_smooth; j++)
			{
				if (j < 0 || j >= len)
					continue;
				sum += line[j];
				nb++;
			}
			newLine[i] = (sum + nb / 2) / nb;
		}
		st i_start = 0;
		while (i_start < len && newLine[i_start] == 0)
			i_start++;
		st i_end = len - 1;
		while (i_end >= 0 && newLine[i_end] == 0)
			i_end--;
		for (st i = i_start; i <= i_end; i++)
			if (newLine[i] == 0)
			{
				line = newLine;
				break;
			}
		// TODO: get coordinates of the two points
	}
}

void test(void)
{
	Image *image = openImage("Images/image_04.jpeg");
	invertImage(image);
	st width = image->width, height = image->height;
	/*
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
	for (st r = 0; r < r_max; r++)
	{
		for (st theta = 0; theta < 360; theta++)
		{
			if (180 <= theta && theta <= 270)
			{
				r_theta[r][theta] = 0;
				continue;
			}
			st x_min = width, x_max = 0, y_max = 0, y_min = height;
			float _sin = sin(theta * PI / 180);
			float _cos = cos(theta * PI / 180);
			total = 0;
			nb_pixels = 0;
			if ((45 <= theta && theta <= 135) || (225 <= theta && theta <= 315))
			{
				for (st x = 0; x < width; x++)
				{
					st _y;
					st y_;
					getY(x, &_y, &y_, r, _cos, _sin);
					for (st y = _y; y <= y_; y++)
					{
						if (y >= height)
							break;
						if (y < 0)
							y = 0;
						if (fabs(r - x * _cos - y * _sin) < 1)
						{
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
			}
			else
			{
				for (st y = 0; y < height; y++)
				{
					st _x;
					st x_;
					getX(&_x, &x_, y, r, _cos, _sin);
					for (st x = _x; x <= x_; x++)
					{
						if (x >= width)
							break;
						if (x < 0)
							x = 0;
						if (fabs(r - x * _cos - y * _sin) < 1)
						{
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
			}
			int value = (nb_pixels && (x_max - x_min > width / 4 || y_max - y_min > height / 4)) ? total / nb_pixels : 0;
			r_theta[r][theta] = value;
			if (value > best_value)
			{
				best_r = r;
				best_theta = theta;
				best_value = value;
			}
			if (!value)
				theta++;
		}
	}
	char *chars = "$@B%8&WM#*oahkbdpqwmZO0QLCJUYXzcvunxrjft/\\|()1{}[]?i+~<>!lI-;:,\"^`_'. ";
	for (st r = 0; r < r_max; r += 15)
	{
		for (st theta = 0; theta < 360; theta++)
		{
			printf("%c", chars[(int)(69 - 69 * ((float)r_theta[r][theta] / best_value))]);
		}
		printf("\n");
	}
	for (st i = 0; i < 20; i++)
	{
		int best_value = 0;
		st best_r = 0, best_theta = 0;
		int value;
		for (st r = 0; r < r_max; r++)
			for (st theta = 0; theta < 360; theta++)
			{
				value = r_theta[r][theta];
				if (value > best_value)
				{
					best_value = value;
					best_r = r;
					best_theta = theta;
				}
			}
		printf("r = %zu, theta = %zu, value = %d\n", best_r, best_theta, best_value);
		int p1x = 0, p1y = best_r / sin(best_theta * PI / 180);
		int p2y = 0, p2x = best_r / cos(best_theta * PI / 180);
		int p3x = width - 1, p3y = (best_r - p3x * cos(best_theta * PI / 180)) / sin(best_theta * PI / 180);
		int p4y = 0, p4x = (best_r - p4y * sin(best_theta * PI / 180)) / cos(best_theta * PI / 180);
		printf("p1 = (%d, %d); p2 = (%d, %d)\n", p1x, p1y, p2x, p2y);
		printf("p3 = (%d, %d); p4 = (%d, %d)\n\n", p3x, p3y, p4x, p4y);
		st len;
		uchar *line = getLine(pixels, width, height, best_r, best_theta, best_value, &len);
		for (st i = 0; i < len; i++)
			printf("%c ", line[i] ? '#' : '.');
		printf("\n");
		for (st r = best_r - 20; r <= best_r + 20; r++)
		{
			if (r >= r_max)
				break;
			if (r < 0)
				continue;
			for (st theta = best_theta - 20; theta <= best_theta + 20; theta++)
			{
				r_theta[r][theta % 360] = 0;
			}
		}
		free(line);
	}
	*/
	return;
}
