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
	saturateImage(resized);
	Quadri *quadri = detectGrid(resized);
	showQuadri(resized, quadri, 255, 0, 0);
	rotateWithView(resized);
	freeQuadri(quadri);
	freeImage(resized);
return 0;
}
