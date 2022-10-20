#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "tools.h"
#include "openImage.h"
#include "transformImage.h"
#include "hough.h"

int main()
{
	Image *image = openImage(FILENAME);
	Image *resized = resizeImage(image, image->width / 2, image->height / 2);
	saturateImage(resized);
	Square *square = detectGrid(resized);
	printf("Square:\n");
	printf("p1: (%zu, %zu)\n", square->p1->x, square->p1->y);
	printf("p2: (%zu, %zu)\n", square->p2->x, square->p2->y);
	printf("p3: (%zu, %zu)\n", square->p3->x, square->p3->y);
	printf("p4: (%zu, %zu)\n", square->p4->x, square->p4->y);
	return 0;
}
