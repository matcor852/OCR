#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "tools.h"
#include "openImage.h"
#include "transformImage.h"
#include "hough.h"
#include "display.h"

int main(int argc, char **argv)
{
	if (argc != 2)
	{
		errx(EXIT_FAILURE, "argument should be path of image");
	}
	const char *path = argv[1];
	Image *image = rotateWithView(path);
	//grayscaleImage(image);
	displayImage(image);
	Image *resized = resizeImage(image, image->width, image->height);
	
	saturateImage(resized);
	displayImage(resized);
	
	Quadri *quadri = detectGrid(resized);
	Segment s1 = {quadri->p1->x, quadri->p1->y, quadri->p3->x, quadri->p3->y, 0, 0, 0};
	Segment s2 = {quadri->p1->x, quadri->p1->y, quadri->p2->x, quadri->p2->y, 0, 0, 0};
	Segment s3 = {quadri->p3->x, quadri->p3->y, quadri->p4->x, quadri->p4->y, 0, 0, 0};
	Segment s4 = {quadri->p2->x, quadri->p2->y, quadri->p4->x, quadri->p4->y, 0, 0, 0};

	Segment *segments[] = {&s1, &s2, &s3 , &s4};
	showLines(image, segments, 0, 0, 255, 4);
	printf("Grid:\n");
	printf("p1: (%zu, %zu)\n", quadri->p1->x, quadri->p1->y);
	printf("p2: (%zu, %zu)\n", quadri->p2->x, quadri->p2->y);
	printf("p3: (%zu, %zu)\n", quadri->p3->x, quadri->p3->y);
	printf("p4: (%zu, %zu)\n", quadri->p4->x, quadri->p4->y);
	freeQuadri(quadri);
	freeImage(resized);
	freeImage(image);
	
return 0;
}
