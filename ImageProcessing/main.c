#include "display.h"
#include "hough.h"
#include "openImage.h"
#include "tools.h"
#include "transformImage.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>

#define WINDOW_WIDTH  800
#define WINDOW_HEIGHT 600

int main(int argc, char *argv[]) {
	if (argc < 2) {
		printf("Usage: %s <image>\n", argv[0]);
		return 1;
	}
	Image *image = openImage(argv[1]);
	Image *resized = autoResize(image, WINDOW_WIDTH, WINDOW_HEIGHT);
	freeImage(image);
	int theta = rotateWithView(resized);
	displayImage(resized);
	calibrateImage(resized, 25);
	displayImage(resized);
	saturateImage(resized);
	Image *rotated = rotateImage(resized, theta, 255);
	freeImage(resized);
	Quadri *quadri = detectGrid(rotated);
	if (quadri == NULL) {
		printf("No grid detected\n");
		freeImage(rotated);
		return 1;
	}
	showQuadri(rotated, quadri, 0, 255, 0);
	freeQuadri(quadri);
	freeImage(rotated);
	return 0;
}
