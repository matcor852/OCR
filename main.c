#include <stdio.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_timer.h>

int main(){
	// SDL_Init(SDL_INIT_EVERYTHING);
	char *filename = "image001.png";
	SDL_Surface* surface  = IMG_Load(filename);
	int width = surface->w, height = surface->h;
	printf("width: %d, height: %d\n", width, height);
	return 0;
}
