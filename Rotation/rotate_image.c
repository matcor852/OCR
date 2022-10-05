#include "rotate_image.h"

//catch all events in an endless loop
void event_loop(SDL_Renderer* renderer, SDL_Texture* colored)
{
    SDL_Event event;
    // Width and height of the window.
    int w,h;
    // Variable of angle of editing
    float angle = 0;
    //Step is add to angle when arrow is pressed
    float step = 2;

    draw(renderer, colored, angle);

    while (1)
    {
        SDL_WaitEvent(&event);
        switch (event.type)
        {
            case SDL_QUIT:  
                return;

            case SDL_WINDOWEVENT:
                if (event.window.event == SDL_WINDOWEVENT_RESIZED)
                    {
                        w = event.window.data1;
                        h = event.window.data2;
                        draw(renderer,colored,angle);
                    }
                break;

            case SDL_KEYDOWN:
                if (event.key.keysym.scancode == SDL_SCANCODE_S)
                    save_texture("edited.png",renderer,colored,w,h);
                if (event.key.keysym.scancode == SDL_SCANCODE_LEFT || event.key.keysym.scancode == SDL_SCANCODE_A)
                    angle = angle - step;
                if (event.key.keysym.scancode == SDL_SCANCODE_RIGHT || event.key.keysym.scancode == SDL_SCANCODE_D)
                    angle = angle + step;
                if (event.key.keysym.scancode == SDL_SCANCODE_ESCAPE)
                    return;
                draw(renderer,colored,angle);
                break;
        }

    }
}

//draws the image with a certain angle "angle"
void draw(SDL_Renderer* renderer, SDL_Texture * texture, double angle)
{ 
    SDL_RenderClear(renderer);
    SDL_RenderCopyEx(renderer, texture, NULL, NULL, angle, NULL, 0);
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

