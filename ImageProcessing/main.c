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
	
	Quadri *quadri = detectGrid(rotated);
	if (quadri == NULL)
	{
		printf("No grid detected\n");
		return 1;
	}
	showQuadri(rotated, quadri, 0, 255, 0);
	freeQuadri(quadri);
	freeImage(rotated);
return 0;
}
