#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <err.h>
#include <string.h>

#define M_PI 3.1415926535

int str_cmp(char * str1, char * str2);

void event_loop(SDL_Renderer* renderer, SDL_Texture* colored);

int rotate(int argc, char ** argv);

int resize(int argc, char ** argv);

void draw(SDL_Renderer* renderer, SDL_Texture * texture, double angle);

void save_renderer(const char* file_name, SDL_Renderer* renderer, SDL_Rect rec_dest);

SDL_Surface * load_image(const char* path);


