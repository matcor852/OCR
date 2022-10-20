#include "display.h"

void showLines(Image *background, Segment **segments, int r, int g, int b, st nb_segments)
{
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
        errx(EXIT_FAILURE, "%s", SDL_GetError());
    SDL_Window *window;
    window = SDL_CreateWindow("Lines preview", 0, 0, 1, 1,
                              SDL_WINDOW_SHOWN);
    if (window == NULL)
        errx(EXIT_FAILURE, "%s", SDL_GetError());
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1,
                                                SDL_RENDERER_SOFTWARE);
    if (renderer == NULL)
        errx(EXIT_FAILURE, "%s", SDL_GetError());
    SDL_Surface *surface = imageToSurface(background);
    if (surface == NULL)
        errx(EXIT_FAILURE, "%s", SDL_GetError());
    SDL_SetWindowSize(window, surface->w, surface->h);
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
    if (texture == NULL)
        errx(EXIT_FAILURE, "%s", SDL_GetError());
    SDL_FreeSurface(surface);
    /// IMAGE AND LINES PRINTER
    int w, h;
    SDL_QueryTexture(texture, NULL, NULL, &w, &h);
    SDL_Rect rec_dest = {0, 0, w, h};
    SDL_Event event;
    SDL_WaitEvent(&event);
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, texture, NULL, &rec_dest);
    SDL_SetRenderDrawColor(renderer, r, g, b, 0);
    for (st i = 0; i < nb_segments; i++)
    {
        SDL_RenderDrawLine(renderer, segments[i]->x1, segments[i]->y1,
                           segments[i]->x2, segments[i]->y2);
    }
    SDL_RenderPresent(renderer);
    /// KEEP DISPLAY RESULT
    int keepDisplay = 1;
    while (keepDisplay)
    {
        SDL_WaitEvent(&event);
        switch (event.type)
        {
        case SDL_QUIT:
            keepDisplay = 0;
            break;
        }
    }
    /// DESTRUCTION
    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    IMG_Quit();
    SDL_Quit();
    return;
}

SDL_Surface *imageToSurface(Image *image)
{
    st w = image->width;
    st h = image->height;
    uc *pixels = image->pixels;
    SDL_Surface *surface = SDL_CreateRGBSurfaceWithFormat(0, w, h, 8, SDL_PIXELFORMAT_RGB888);
    SDL_LockSurface(surface);
    for (st y = 0; y < h; y++)
    {
        for (st x = 0; x < w; x++)
        {
            unsigned int value = pixels[y * w + x];
            unsigned int *pixel = surface->pixels + y * surface->pitch + x * surface->format->BytesPerPixel;
            *pixel = (value << 16) | (value << 8) | value;
        }
    }
    SDL_UnlockSurface(surface);
    if (surface == NULL)
        errx(EXIT_FAILURE, "%s", SDL_GetError());
    return surface;
}

int displayImage(Image *image)
{
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
        errx(EXIT_FAILURE, "%s", SDL_GetError());
    SDL_Window *window;
    window = SDL_CreateWindow("Image visualizer", 0, 0, 1, 1,
                              SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);

    if (window == NULL)
        errx(EXIT_FAILURE, "%s", SDL_GetError());
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1,
                                                SDL_RENDERER_ACCELERATED);
    if (renderer == NULL)
        errx(EXIT_FAILURE, "%s", SDL_GetError());
    SDL_Surface *surface = imageToSurface(image);
    if (surface == NULL)
        errx(EXIT_FAILURE, "%s", SDL_GetError());
    SDL_SetWindowSize(window, surface->w, surface->h);
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
    if (texture == NULL)
        errx(EXIT_FAILURE, "%s", SDL_GetError());
    SDL_FreeSurface(surface);

    /// KEEP DISPLAY RESULT
    int keepDisplay = 1;
    while (keepDisplay)
    {
        SDL_Event event;
        SDL_WaitEvent(&event);
        switch (event.type)
        {
        case SDL_WINDOWEVENT:
            if (event.window.event == SDL_WINDOWEVENT_RESIZED)
                draw(renderer, texture);
            break;
        case SDL_QUIT:
            keepDisplay = 0;
            break;
        }
    }
    /// KEEP DISPLAY RESULT

    // DESTRUCTION
    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    IMG_Quit();
    SDL_Quit();

    return EXIT_SUCCESS;
}

// catch all events in an endless loop
void event_loop(SDL_Renderer *renderer, Image *image)
{
    SDL_Surface *surface = imageToSurface(image);
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
    if (texture == NULL)
        errx(EXIT_FAILURE, "%s", SDL_GetError());
    SDL_Event event;
    int angle = 0;
    int step = 5;
    Image *rotated = rotateImage(image, angle);
	int running = 1;
    while (running)
    {
        SDL_WaitEvent(&event);
        switch (event.type)
        {
        case SDL_QUIT:
			running = 0;
        case SDL_WINDOWEVENT:
            if (event.window.event == SDL_WINDOWEVENT_RESIZED)
                draw(renderer, texture);
            break;
        case SDL_KEYDOWN:
            if (event.key.keysym.scancode == SDL_SCANCODE_S)
                saveImage(rotated, "rotated.png");
            if (event.key.keysym.scancode == SDL_SCANCODE_LEFT ||
                event.key.keysym.scancode == SDL_SCANCODE_RIGHT)
            {
                if (event.key.keysym.scancode == SDL_SCANCODE_LEFT)
                    angle += step;
                else
                    angle -= step;
				angle %= 360;
				freeImage(rotated);
				rotated = rotateImage(image, angle);
				SDL_DestroyTexture(texture);
				SDL_FreeSurface(surface);
                surface = imageToSurface(rotated);
                texture = SDL_CreateTextureFromSurface(renderer, surface);
                if (texture == NULL)
                    errx(EXIT_FAILURE, "%s", SDL_GetError());
                draw(renderer, texture);
            }
            if (event.key.keysym.scancode == SDL_SCANCODE_ESCAPE)
				running = 0;
            break;
        }
    }
	freeImage(rotated);
    SDL_DestroyTexture(texture);
    SDL_FreeSurface(surface);
}

void draw(SDL_Renderer *renderer, SDL_Texture *texture)
{
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, texture, NULL, NULL);
    SDL_RenderPresent(renderer);
}

int rotateWithView(const char *filename)
{
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
        errx(EXIT_FAILURE, "%s", SDL_GetError());
    SDL_Window *window;
    window = SDL_CreateWindow("Image visualizer", 0, 0, 1, 1,
                              SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
    if (window == NULL)
        errx(EXIT_FAILURE, "%s", SDL_GetError());
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1,
                                                SDL_RENDERER_SOFTWARE);
    if (renderer == NULL)
        errx(EXIT_FAILURE, "%s", SDL_GetError());
    Image *image = openImage(filename);
    SDL_SetWindowSize(window, image->width, image->height);
    // MAIN
    event_loop(renderer, image);
    // DESTRUCTION
	freeImage(image);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    IMG_Quit();
    SDL_Quit();
    return EXIT_SUCCESS;
}
