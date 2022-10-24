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
	Image * image= openImage(argv[1]);
	Image *resized = resizeImage(image, image->width/2, image->height/2);

	Image *rotated = rotateWithView(resized);
	freeImage(resized);

	Quadri *quadri = detectGrid(rotated);
	if (quadri == NULL)
	{
		printf("No grid detected\n");
	}
	else
	{
		showQuadri(rotated, quadri, 0, 255, 0);
		freeQuadri(quadri);
	}
	freeImage(rotated);
	freeImage(image);

return 0;
}
