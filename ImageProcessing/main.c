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
	//Image *image = openImage(FILENAME);
	//Image *resized = resizeImage(image, image->width / 2, image->height / 2);
	//saturateImage(resized);
	rotateWithView(FILENAME);
	return 0;
}
