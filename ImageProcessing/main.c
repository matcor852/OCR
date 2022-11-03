#include "display.h"
#include "hough.h"
#include "matrices.h"
#include "openImage.h"
#include "tools.h"
#include "transformImage.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <string.h>
#include <err.h>

#define WINDOW_WIDTH  800
#define WINDOW_HEIGHT 600

char *cleanPath(char *filename, char *dest) {
	char *slash = strrchr(filename, '/');
	if (slash == NULL) {
		strcpy(dest, filename);
	} else {
		strcpy(dest, slash + 1);
	}
	char *dot = strrchr(dest, '.');
	if (!dot || dot == dest) return dest;
	*dot = '\0';
	return dest;
}

void printHelp(char *exeName) {
	printf("%s --rotate <image> <angle>: rotate the image <image> with <angle> angle.\n", exeName);
	printf("%s --rotateView <image>: rotate the image <image> with a preview (use arrow keys).\n", exeName);
	printf("%s --demo <image>: see full demo.\n", exeName);
	printf("%s --test <image> [options]: test the image <image> with the given options.\n", exeName);
}

int missingArg(char *exeName, char *command) {
	printf("Missing argument for %s.\n", command);
	printHelp(exeName);
	return 1;
}

void exeRotate(char *filename, int angle) {
	Image *image = openImage(filename);
	Image *rotated = rotateImage(image, angle, 0);
	char filenameStripped[30];
	cleanPath(filename, filenameStripped);
	char destName[40];
	sprintf(destName, "%s_r%i.png", filenameStripped, angle);
	saveImage(rotated, destName);
	freeImage(rotated);
	freeImage(image);
}

void exeRotateView(char *filename) {
	Image *image = openImage(filename);
	int theta = rotateWithView(image);
	Image *rotated = rotateImage(image, theta, 0);
	freeImage(image);
	char filenameStripped[30];
	cleanPath(filename, filenameStripped);
	char destName[40];
	sprintf(destName, "%s_r%i.png", filenameStripped, theta);
	saveImage(rotated, destName);
	freeImage(rotated);
}

void exeDemo(char *filename) {
	// open image
	Image *image = openImage(filename);
	Image *resized = autoResize(image, WINDOW_WIDTH, WINDOW_HEIGHT);
	freeImage(image);
	// rotate image
	int theta = rotateWithView(resized);
	Image *rotated = rotateImage(resized, theta, 255);
	freeImage(resized);
	// preprocess image
	saturateImage(rotated);
	// detect grid
	Quadri *quadri = detectGrid(rotated);
	if (quadri == NULL) {
		freeImage(rotated);
		errx(1, "No grid detected.");
	}
	// display results
	showQuadri(rotated, quadri, 0, 255, 0);
	Image *extracted = extractGrid(rotated, quadri, 900, 900);
	freeImage(rotated);
	freeQuadri(quadri);
	displayImage(extracted, "Extracted grid");
	// save image
	char filenameStripped[30];
	cleanPath(filename, filenameStripped);
	saveBoard(extracted, filenameStripped);
	freeImage(extracted);
}

void exeTest(char *filename, int radius) {
	Image *image = openImage(filename);
	Image *resized = autoResize(image, WINDOW_WIDTH, WINDOW_HEIGHT);
	freeImage(image);
	displayImage(resized, "Original image");
	gaussianBlur(resized);
	calibrateImage(resized, radius);
	displayImage(resized, "Saturated");
	freeImage(resized);
}

int main(int argc, char *argv[]) {
	char *exeName = argv[0];
	if (argc < 2) {
		printHelp(exeName);
		return 1;
	}
	for (int i = 1; i < argc; i++) {
		char *command = argv[i];
		if (!strcmp(command, "-h") || !strcmp(command, "--help")) {
			printHelp(exeName);
			return 0;
		}
		else if (!strcmp(command, "--rotate")) {
			if (i + 2 >= argc) return missingArg(exeName, command);
			char *filename = argv[++i];
			int angle = atoi(argv[++i]);
			exeRotate(filename, angle);
		}
		else if (!strcmp(command, "--rotateView")) {
			if (i + 1 >= argc) return missingArg(exeName, command);
			char *filename = argv[++i];
			exeRotateView(filename);
		}
		else if (!strcmp(command, "--demo")) {
			if (i + 1 >= argc) return missingArg(exeName, command);
			char *filename = argv[++i];
			exeDemo(filename);
		}
		else if (!strcmp(command, "--test")) {
			if (i + 2 >= argc) return missingArg(exeName, command);
			char *filename = argv[++i];
			int radius = atoi(argv[++i]);
			exeTest(filename, radius);
		}
		else {
			printf("Unknown command %s.\n", command);
			printHelp(exeName);
			return 1;
		}
	}
	return 0;
}
