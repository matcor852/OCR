#include "rotate_image.h"

//draws the image with a certain angle "angle"
void draw(SDL_Renderer* renderer, SDL_Texture * texture, double angle)
{ 
    SDL_RenderClear(renderer);
    SDL_RenderCopyEx(renderer, texture , NULL, NULL, angle, NULL, 0);
    SDL_RenderPresent(renderer);
}

//it saves a texture in a png file named filename according to a certain width and height
void save_texture(const char* file_name, SDL_Renderer* renderer, SDL_Texture* texture, int w, int h) 
{
    SDL_Texture* target = SDL_GetRenderTarget(renderer);
    SDL_SetRenderTarget(renderer, texture);
    SDL_Surface* surface = SDL_CreateRGBSurface(0, w, h, 32, 0, 0, 0, 0);
    SDL_RenderReadPixels(renderer, NULL, surface->format->format, surface->pixels, surface->pitch);
    IMG_SavePNG(surface, file_name);
    SDL_FreeSurface(surface);
    SDL_SetRenderTarget(renderer, target);
}

// Loads an image in a surface.
// The format of the surface is SDL_PIXELFORMAT_RGB888.
// path: Path of the image.
SDL_Surface* load_image(const char* path)
{
    SDL_Surface * temp = IMG_Load(path);
    if (temp == NULL)
        errx(EXIT_FAILURE, "%s", SDL_GetError());
    SDL_Surface * res = SDL_ConvertSurfaceFormat(temp, SDL_PIXELFORMAT_RGB888, 0);
    if (res == NULL)
        errx(EXIT_FAILURE, "%s", SDL_GetError());
    SDL_FreeSurface(temp);
    return res;
}

// Converts a colored pixel into grayscale.
// pixel_color: Color of the pixel to convert in the RGB format.
// format: Format of the pixel used by the surface.
Uint32 pixel_to_grayscale(Uint32 pixel_color, SDL_PixelFormat* format)
{
    Uint8 r, g, b;
    SDL_GetRGB(pixel_color, format, &r, &g, &b);
    Uint8 avrg = 0.33*r + 0.33*g + 0.33*b;
    Uint32 color = SDL_MapRGB(format, avrg , avrg , avrg );
    return color;
}

