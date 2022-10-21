#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "tools.h"
#include "openImage.h"
#include "transformImage.h"
#include "hough.h"
#include "display.h"

int main()
{
	Image *image = openImage(FILENAME);
	Image *resized = resizeImage(image, image->width / 2, image->height / 2);
	freeImage(image);
	saturateImage(resized);
	Quadri *quadri = detectGrid(resized);
	/*
	Segment s1 = {quadri->p1->x, quadri->p1->y, quadri->p3->x, quadri->p3->y, 0, 0, 0};
	Segment s2 = {quadri->p1->x, quadri->p1->y, quadri->p2->x, quadri->p2->y, 0, 0, 0};
	Segment s3 = {quadri->p3->x, quadri->p3->y, quadri->p4->x, quadri->p4->y, 0, 0, 0};
	Segment s4 = {quadri->p2->x, quadri->p2->y, quadri->p4->x, quadri->p4->y, 0, 0, 0};
	Segment *segments[] = {&s1, &s2, &s3 , &s4};
	showLines(resized, segments, 255, 0, 0, 4);
	*/
	printf("Grid:\n");
	printf("p1: (%zu, %zu)\n", quadri->p1->x, quadri->p1->y);
	printf("p2: (%zu, %zu)\n", quadri->p2->x, quadri->p2->y);
	printf("p3: (%zu, %zu)\n", quadri->p3->x, quadri->p3->y);
	printf("p4: (%zu, %zu)\n", quadri->p4->x, quadri->p4->y);
	freeQuadri(quadri);
	freeImage(resized);
	rotateWithView(FILENAME);
return 0;
}
