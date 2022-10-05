#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <err.h>
#include <string.h>
#define M_PI 3.1415926535

void event_loop(SDL_Renderer* renderer, SDL_Texture* colored);
void draw(SDL_Renderer* renderer, SDL_Texture * texture, double angle);
void save_texture(const char* file_name, SDL_Renderer* renderer, SDL_Texture* texture, int w, int h);
SDL_Surface* load_image(const char* path);
Uint32 pixel_to_grayscale(Uint32 pixel_color, SDL_PixelFormat* format);


