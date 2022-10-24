#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "tools.h"
#include "openImage.h"
#include "transformImage.h"
#include "hough.h"
#include "display.h"

int main(int argc, char *argv[])
{
	if (argc < 2)
	{
		printf("Usage: %s <image>\n", argv[0]);
		return 1;
	}
	Image *image = openImage(argv[1]);
	Image *resized = resizeImage(image, image->width / 4, image->height / 4);
	freeImage(image);
	Image *rotated = rotateWithView(resized);
	freeImage(resized);
	saturateImage(rotated);
	displayImage(rotated);
	
	Quadri *quadri = detectGrid(rotated);
	showQuadri(rotated, quadri, 255, 0, 0);
	rotateWithView(rotated);
	freeQuadri(quadri);
	freeImage(rotated);
return 0;
}
