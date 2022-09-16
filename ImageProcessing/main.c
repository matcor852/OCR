#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "openImage.h"

int main(){
	char *filename = "Images/image001.png";
	size_t width, height;
	uchar **pixels = NULL;
	openImage(filename, &width, &height, pixels);
	//SDL_Surface* surface = openImage(filename, &width, &height);
	//int width = surface->w, height = surface->h;
	return 0;
}
