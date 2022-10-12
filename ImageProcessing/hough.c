#include <stdio.h>
#include <math.h>
#include "tools.h"
#include "openImage.h"
#include "transformImage.h"
#define PI 3.141592654

typedef struct
{
	st x1, y1, x2, y2, theta, length;
} Segment;

int isVertical(st theta)
{
	return (0 <= theta && theta < 45) ||
		   (135 <= theta && theta < 225) ||
		   (315 <= theta && theta < 360);
}

void fillR_thetaVertical(Image *image, uc *r_theta, st r_max, st theta, float _cos, float _sin)
{
	st w = image->width, h = image->height;
	uc *pixels = image->pixels;
	int value;
	int nb;
	for (st r = 0; r < r_max; r++)
	{
		value = 0;
		nb = 0;
		for (st y = 0; y < h; y++)
		{
			st x = (r - y * _sin) / _cos;
			if (x >= 0 && x < w)
			{
				value += pixels[y * w + x];
				nb++;
			}
		}
		r_theta[r * 360 + theta] = nb ? value / nb : 0;
	}
}

void fillR_thetaHorizontal(Image *image, uc *r_theta, st r_max, st theta, float _cos, float _sin)
{
	st w = image->width, h = image->height;
	uc *pixels = image->pixels;
	int value;
	int nb;
	for (st r = 0; r < r_max; r++)
	{
		value = 0;
		nb = 0;
		for (st x = 0; x < w; x++)
		{
			st y = (r - x * _cos) / _sin;
			if (y >= 0 && y < h)
			{
				value += pixels[y * w + x];
				nb++;
			}
		}
		r_theta[r * 360 + theta] = nb ? value / nb : 0;
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
		if (theta == 180)
		{
			theta = 270;
			vertical = 0;
		}
		if (theta == 45 || theta == 135 || theta == 315)
			vertical = !vertical;
		_cos = cos(theta * PI / 180), _sin = sin(theta * PI / 180);
		(vertical ? fillR_thetaVertical : fillR_thetaHorizontal)(image, r_theta, r_max, theta, _cos, _sin);
	}
	for (st r = 0; r < r_max; r++)
		for (st theta = 180; theta < 270; theta++)
			r_theta[r * 360 + theta] = 0;
}

void printR_theta(uc *r_theta, st r_max)
{
	// display theta [270 - 360[ + [0 - 180[
	static char *chars = " .'_`^\",:;I-!<>~+i?][}{1()|/\\ftjrxnuvczXYUJCLQ0OZmwqpdbkhao*#MW&8%B@";
	for (st r = 0; r < r_max; r += 15)
	{
		for (st theta = 270; theta != 180; theta++)
		{
			if (theta == 360)
				theta = 0;
			printf("%c", chars[r_theta[r * 360 + theta] * 70 / 256]);
		}
		printf("\n");
	}
}

void getVerticalLine(Image *image, st r, st theta, uc *line)
{
	uc *pixels = image->pixels;
	st width = image->width;
	st height = image->height;
	float _cos = cos(theta * PI / 180);
	float _sin = sin(theta * PI / 180);
	for (st y = 0; y < height; y++)
	{
		st x = (r - y * _sin) / _cos;
		if (x < 0 || x >= width)
			line[y] = 0;
		else
			line[y] = pixels[y * width + x];
	}
}

void getHorizontalLine(Image *image, st r, st theta, uc *line)
{
	uc *pixels = image->pixels;
	st width = image->width;
	st height = image->height;
	float _cos = cos(theta * PI / 180);
	float _sin = sin(theta * PI / 180);
	for (st x = 0; x < width; x++)
	{
		st y = (r - x * _cos) / _sin;
		if (y < 0 || y >= height)
			line[x] = 0;
		else
			line[x] = pixels[y * width + x];
	}
}

void smoothLine(uc *line, st len, st *i_start, st *i_end)
{
	// Gets the median as threshold
	uc histo[256] = {0};
	for (st i = 0; i < len; i++)
		histo[line[i]]++;
	st total = 0;
	uc threshold = 0;
	while (total < len / 2)
		total += histo[threshold++];
	for (st i = 0; i < len; i++)
		line[i] = line[i] > threshold ? 1 : 0;
	// Fills the gaps smaller than min_gap
	int change = 1;
	st min_gap = 2.5 / 100 * len;
	while (change)
	{
		change = 0;
		for (size_t i = 0; i < len; i++)
		{
			if (line[i] == 0)
			{
				st j = i;
				while (j < len && line[j] == 0)
					j++;
				if (j - i < min_gap)
				{
					for (st k = i; k < j; k++)
						line[k] = 1;
					change = 1;
				}
				i = j;
			}
		}
	}
	// Keeps only the longest line
	st max_len = 0;
	st max_start = 0;
	for (size_t i = 0; i < len; i++)
	{
		if (line[i] == 1)
		{
			st j = i;
			while (j < len && line[j] == 1)
				j++;
			if (j - i > max_len)
			{
				max_len = j - i;
				max_start = i;
			}
			i = j;
		}
	}
	for (size_t i = 0; i < len; i++)
		line[i] = i >= max_start && i < max_start + max_len ? 1 : 0;
	*i_start = max_start;
	*i_end = max_start + max_len;
}

void deleteBest(uc *r_theta, st r_max, st best_r, st best_theta)
{
	st theta_min = (best_theta + 345) % 360;
	st theta_max = (best_theta + 15) % 360;
	st r_min = best_r - 15 < 0 ? 0 : best_r - 15;
	r_max = best_r + 15 > r_max ? r_max : best_r + 15;
	for (st r = r_min; r < r_max; r++)
	{
		for (st theta = theta_min; theta != theta_max; theta++)
		{
			if (theta == 360)
				theta = 0;
			r_theta[r * 360 + theta] = 0;
		}
	}
}

Segment *getBestSegment(uc *r_theta, st r_max, Image *image)
{
	st best_value = 0;
	st best_r = 0;
	st best_theta = 0;
	for (st r = 0; r < r_max; r++)
	{
		for (st theta = 270; theta != 180; theta++)
		{
			if (theta == 360)
				theta = 0;
			if (r_theta[r * 360 + theta] > best_value)
			{
				best_value = r_theta[r * 360 + theta];
				best_r = r;
				best_theta = theta;
			}
		}
	}
	if (best_value == 0)
		return NULL;
	int vertical = isVertical(best_theta);
	st height = image->height, width = image->width;
	st len = vertical ? height : width;
	uc line[len];
	if (vertical)
		getVerticalLine(image, best_r, best_theta, line);
	else
		getHorizontalLine(image, best_r, best_theta, line);
	st i_start, i_end;
	smoothLine(line, len, &i_start, &i_end);
	printf("i_start = %zu, i_end = %zu\n", i_start, i_end);
	float _cos = cos(best_theta * PI / 180);
	float _sin = sin(best_theta * PI / 180);
	int x1, y1, x2, y2;
	if (vertical)
	{
		y1 = 0;
		x1 = (best_r - y1 * _sin) / _cos;
		y2 = height - 1;
		x2 = (best_r - y2 * _sin) / _cos;
	}
	else
	{
		x1 = 0;
		y1 = (best_r - x1 * _cos) / _sin;
		x2 = width - 1;
		y2 = (best_r - x2 * _cos) / _sin;
	}
	printf("x1 = %d, y1 = %d, x2 = %d, y2 = %d\n", x1, y1, x2, y2);
	float ratio_start = (float)i_start / len;
	float ratio_end = (float)i_end / len;
	st x_start = x1 + ratio_start * (x2 - x1);
	st y_start = y1 + ratio_start * (y2 - y1);
	st x_end = x1 + ratio_end * (x2 - x1);
	st y_end = y1 + ratio_end * (y2 - y1);
	Segment *segment = malloc(sizeof(Segment));
	segment->x1 = x_start;
	segment->y1 = y_start;
	segment->x2 = x_end;
	segment->y2 = y_end;
	segment->theta = best_theta;
	segment->length = sqrt(pow(x_end - x_start, 2) + pow(y_end - y_start, 2));
	deleteBest(r_theta, r_max, best_r, best_theta);
	printf("Segment: (%zu, %zu), (%zu, %zu)\n", x_start, y_start, x_end, y_end);
	printf("Theta: %zu\n", best_theta);
	printf("Length: %zu\n", segment->length);
	if (segment->length < len / 4)
	{
		printf("Segment too short\n");
		free(segment);
		return NULL;
	}
	return segment;
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
	printR_theta(r_theta, r_max);
	Segment **segments = malloc(sizeof(Segment *) * 50);
	st nb_Segments = 0;
	for (st i = 0; i < 50; i++)
	{
		Segment *segment = getBestSegment(r_theta, r_max, image);
		printf("\n");
		if (segment != NULL)
		{
			segments[nb_Segments] = segment;
			nb_Segments++;
		}
	}
	segments = realloc(segments, sizeof(Segment *) * nb_Segments);
	printf("width = %zu, height = %zu\n", width, height);
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
