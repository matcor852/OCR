#include <stdio.h>
#include <math.h>
#include "tools.h"
#include "openImage.h"
#include "transformImage.h"
#define PI 3.141592654

void fillR_thetaVertical(Image *image, uc *r_theta, st r_max, st theta, float _cos, float _sin)
{
	st w = image->width, h = image->height;
	uc *pixels = image->pixels;
	int value;
	int nb = 0;
	for (st r = 0; r < r_max; r++)
	{
		value = 0;
		for (st y = 0; y < h; y++)
		{
			st x = (r - y * _sin) / _cos;
			if (x >= 0 && x < w)
			{
				value += pixels[y * w + x];
				nb++;
			}
		}
		r_theta[r * 360 + theta] = (value + nb / 2) / nb;
	}
}

void fillR_thetaHorizontal(Image *image, uc *r_theta, st r_max, st theta, float _cos, float _sin)
{
	st w = image->width, h = image->height;
	uc *pixels = image->pixels;
	int value;
	int nb = 0;
	for (st r = 0; r < r_max; r++)
	{
		value = 0;
		for (st x = 0; x < w; x++)
		{
			st y = (r - x * _cos) / _sin;
			if (y >= 0 && y < h)
			{
				value += pixels[y * w + x];
				nb++;
			}
		}
		r_theta[r * 360 + theta] = (value + nb / 2) / nb;
	}
}

void fillR_theta(Image *image, uc *r_theta, st r_max)
{
	int vertical = 1;
	float _cos, _sin;
	for (st theta = 0; theta < 360; theta++)
	{
		// vertical: [0, 45[, [135, 180[, [315, 360[
		// horizontal: [45, 135[, [270, 315[
		// skip useless angles (180 to 270)
		if (theta == 180)
		{
			theta = 270;
			vertical = 0;
		}
		// switch from vertical to horizontal and vice versa
		if (theta == 45 || theta == 135 || theta == 315)
			vertical = !vertical;
		_cos = cos(theta), _sin = sin(theta);
		(vertical ? fillR_thetaVertical : fillR_thetaHorizontal)(image, r_theta, r_max, theta, _cos, _sin);
	}
	// fill from 180 to 270 with 0
	for (st r = 0; r < r_max; r++)
		for (st theta = 180; theta < 270; theta++)
			r_theta[r * 360 + theta] = 0;
}

/*
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
*/

void printR_theta(uc *r_theta, st r_max)
{
	char *chars = "$@B%8&WM#*oahkbdpqwmZO0QLCJUYXzcvunxrjft/\\|()1{}[]?i+~<>!lI-;:,\"^`_'. ";
	for (st r = 0; r < r_max; r += 15)
	{
		for (st theta = 0; theta < 360; theta++)
		{
			printf("%c", chars[(int)(69 - 69 * ((float)r_theta[r * 360 + theta] / 255))]);
		}
		printf("\n");
	}
}

void detectGrid()
{
	Image *image = openImage("Images/image_04.jpeg");
	invertImage(image);
	st width = image->width, height = image->height;
	st r_max = sqrt(width * width + height * height);
	if (r_max == 0)
		return;
	uc r_theta[r_max * 360];
	fillR_theta(image, r_theta, r_max);
	/*
	
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
