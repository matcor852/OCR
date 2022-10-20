#include <stdio.h>
#include <math.h>
#include "tools.h"
#include "openImage.h"
#include "transformImage.h"
#include "display.h"

#define PI 3.141592654

#define SMOOTH_ERROR 1.5

#define RANGE_DEL_R 15
#define RANGE_DEL_THETA 3

#define NB_SEGMENTS 50
#define COORD_ERROR 3.0
#define ANGLE_ERROR 10
#define LENGTH_ERROR 2

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
			theta = 271;
			vertical = 0;
		}
		if (theta == 45 || theta == 135 || theta == 315)
			vertical = !vertical;
		_cos = cos(theta * PI / 180), _sin = sin(theta * PI / 180);
		(vertical ? fillR_thetaVertical : fillR_thetaHorizontal)(image, r_theta, r_max, theta, _cos, _sin);
	}
	for (st r = 0; r < r_max; r++)
		for (st theta = 180; theta <= 270; theta++)
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

void smoothLine(uc *line, st threshold, st len, st *i_start, st *i_end)
{
	// Gets the best_value as the threshold
	for (st i = 0; i < len; i++)
		line[i] = line[i] >= threshold ? 1 : 0;
	// Fills the gaps smaller than min_gap
	int change = 1;
	st min_gap = SMOOTH_ERROR / 100.0 * len;
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
	st r_min = best_r - RANGE_DEL_R > best_r ? 0 : best_r - RANGE_DEL_R;
	r_max = best_r + RANGE_DEL_R > r_max ? r_max : best_r + RANGE_DEL_R;
	for (st r = r_min; r < r_max; r++)
		r_theta[r * 360 + best_theta] = 0;
}

st st_pow(st a, st b)
{
	return a > b ? pow(a - b, 2) : pow(b - a, 2);
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
	st length = vertical ? height : width;
	uc line[length];
	if (vertical)
		getVerticalLine(image, best_r, best_theta, line);
	else
		getHorizontalLine(image, best_r, best_theta, line);
	st i_start, i_end;
	smoothLine(line, best_value, length, &i_start, &i_end);
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
	float ratio_start = (float)i_start / length;
	float ratio_end = (float)i_end / length;
	st x_start = x1 + ratio_start * (x2 - x1);
	st y_start = y1 + ratio_start * (y2 - y1);
	st x_end = x1 + ratio_end * (x2 - x1);
	st y_end = y1 + ratio_end * (y2 - y1);
	r_theta[best_r * 360 + best_theta] = 0;
	if (x_start >= width || x_end >= width || y_start >= height || y_end >= height)
		return NULL;
	st lenth_segment = sqrt(st_pow(x_end, x_start) + st_pow(y_end, y_start));
	if (lenth_segment < length / 4)
		return NULL;
	deleteBest(r_theta, r_max, best_r, best_theta);
	Segment *segment = malloc(sizeof(Segment));
	segment->x1 = x_start;
	segment->y1 = y_start;
	segment->x2 = x_end;
	segment->y2 = y_end;
	segment->theta = best_theta;
	segment->r = best_r;
	segment->length = lenth_segment;
	return segment;
}

void swapPoints(Segment *segment)
{
	st tmp = segment->x1;
	segment->x1 = segment->x2;
	segment->x2 = tmp;
	tmp = segment->y1;
	segment->y1 = segment->y2;
	segment->y2 = tmp;
}

void printSegment(Segment *segment, st i)
{
	printf("Segment%zu: (%zu, %zu) (%zu, %zu)\n", i, segment->x1, segment->y1, segment->x2, segment->y2);
}

void inersection(Segment *segment1, Segment *segment2, st *x, st *y)
{
	float c1 = cos(segment1->theta), s1 = sin(segment1->theta);
	float c2 = cos(segment2->theta), s2 = sin(segment2->theta);
	int r1 = segment1->r, r2 = segment2->r;
	float det = c1 * s2 - c2 * s1;
	*x = (r1 * s2 - r2 * s1) / det;
	*y = (r2 * c1 - r1 * c2) / det;
	// TODO: test and implement
}

Square *detectGrid(Image *image)
{
	//Image *image = openImage(FILENAME);
	invertImage(image);
	st width = image->width, height = image->height;
	st r_max = sqrt(width * width + height * height);
	if (r_max == 0)
		return NULL;
	uc r_theta[r_max * 360];
	fillR_theta(image, r_theta, r_max);
	Segment *segments[NB_SEGMENTS];
	st i = 0;
	while (i < NB_SEGMENTS)
	{
		Segment *segment = getBestSegment(r_theta, r_max, image);
		if (segment != NULL)
		{
			segments[i] = segment;
			i++;
			// printf("Segment: (%zu, %zu), (%zu, %zu)\n", segment->x1, segment->y1, segment->x2, segment->y2);
			// printf("Theta: %zu\n", segment->theta);
			// printf("Length: %zu\n\n", segment->length);
		}
	}
	showLines(FILENAME, segments, 255, 0, 0, NB_SEGMENTS);
	st min_dist = pow(COORD_ERROR / 100.0 * r_max, 2);
	for (st i1 = 0; i1 < NB_SEGMENTS; i1++)
	{
		Segment *segment1 = segments[i1];
		st segments2[NB_SEGMENTS];
		st j = 0;
		for (st i2 = 0; i2 < NB_SEGMENTS; i2++)
		{
			if (i2 == i1)
				continue;
			Segment *segment2 = segments[i2];
			if (segment1->length * LENGTH_ERROR < segment2->length ||
				segment2->length * LENGTH_ERROR < segment1->length)
				continue;
			int diff_theta = (-90 + segment1->theta - segment2->theta) % 180;
			if (diff_theta > ANGLE_ERROR && diff_theta < 180 - ANGLE_ERROR)
				continue;
			st diff_coord1 = st_pow(segment1->x1, segment2->x1) +
							  st_pow(segment1->y1, segment2->y1);
			st diff_coord2 = st_pow(segment1->x1, segment2->x2) +
							  st_pow(segment1->y1, segment2->y2);
			if (diff_coord2 <= min_dist)
				swapPoints(segment2);
			else if (diff_coord1 > min_dist)
				continue;
			segments2[j] = i2;
			j++;
			//printf("cmp1-2 : (%zu, %zu) (%zu, %zu)\n", segment1->x1, segment1->y1, segment2->x1, segment2->y1);
		}
		segments2[j] = NB_SEGMENTS;

		j = 0;
		st segments3[NB_SEGMENTS];
		for (st i3 = 0; i3 < NB_SEGMENTS; i3++)
		{
			if (i3 == i1)
				continue;
			Segment *segment3 = segments[i3];
			if (segment1->length * LENGTH_ERROR < segment3->length ||
				segment3->length * LENGTH_ERROR < segment1->length)
				continue;
			int diff_theta = (-90 + segment1->theta - segment3->theta) % 180;
			if (diff_theta > ANGLE_ERROR && diff_theta < 180 - ANGLE_ERROR)
				continue;
			st diff_coord1 = st_pow(segment1->x2, segment3->x1) +
							  st_pow(segment1->y2, segment3->y1);
			st diff_coord2 = st_pow(segment1->x2, segment3->x2) +
							  st_pow(segment1->y2, segment3->y2);
			//printf("cmp1-3 : (%zu, %zu) (%zu, %zu)\n", segment1->x2, segment1->y2, segment3->x1, segment3->y1);
			if (diff_coord2 <= min_dist)
				swapPoints(segment3);
			else if (diff_coord1 > min_dist)
				continue;
			segments3[j] = i3;
			j++;
		}
		segments3[j] = NB_SEGMENTS;

		for (st i4 = 0; i4 < NB_SEGMENTS; i4++)
		{
			if (i4 == i1)
				continue;
			st k;
			for (k = 0; segments2[k] != NB_SEGMENTS; k++)
			{
				if (i4 == segments2[k])
					break;
			}
			if (segments2[k] != NB_SEGMENTS)
				continue;
			for (k = 0; segments3[k] != NB_SEGMENTS; k++)
			{
				if (i4 == segments3[k])
					break;
			}
			if (segments3[k] != NB_SEGMENTS)
				continue;
			Segment *segment4 = segments[i4];
			for (st i2 = 0; segments2[i2] != NB_SEGMENTS; i2++)
			{
				Segment *segment2 = segments[segments2[i2]];
				st diff_coord1 = st_pow(segment2->x2, segment4->x1) +
								 st_pow(segment2->y2, segment4->y1);
				st diff_coord2 = st_pow(segment2->x2, segment4->x2) +
								 st_pow(segment2->y2, segment4->y2);
				if (diff_coord2 <= min_dist)
					swapPoints(segment4);
				else if (diff_coord1 > min_dist)
					continue;
				//printf("cmp2-4 : (%zu, %zu) (%zu, %zu)\n", segment2->x2, segment2->y2, segment4->x1, segment4->y1);
				for (st i3 = 0; segments3[i3] != NB_SEGMENTS; i3++)
				{
					Segment *segment3 = segments[segments3[i3]];
					st diff_coord3 = st_pow(segment3->x2, segment4->x2) +
								 	 st_pow(segment3->y2, segment4->y2);
					if (diff_coord3 > min_dist)
						continue;
					//printf("cmp3-4 : (%zu, %zu) (%zu, %zu)\n", segment3->x2, segment3->y2, segment4->x2, segment4->y2);
					Square *square = (Square *)malloc(sizeof(Square));
					square->s1 = segment1;
					square->s2 = segment2;
					square->s3 = segment3;
					square->s4 = segment4;
					return square;
				}
			}
		}
	}
	return NULL;
}
