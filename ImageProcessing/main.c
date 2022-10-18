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
	if (square == NULL)
		printf("No Square found!\n");
	else
		//printf("Square:\n(%zu, %zu)\n(%zu, %zu)\n(%zu, %zu)\n(%zu, %zu)\n", square->x1, square->y1, square->x2, square->y2, square->x3, square->y3, square->x4, square->y4);
	return 0;
}
