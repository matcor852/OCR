#include "display.h"

void showLines(const char * filename, Segment ** segments,int r, int g, int b, st nb_segments)
{
    /// SDL STUFF
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
        errx(EXIT_FAILURE, "%s", SDL_GetError());

    SDL_Window* window;
    window = SDL_CreateWindow("Lines preview", 0, 0, 1, 1,
    SDL_WINDOW_SHOWN);
    
    if (window == NULL)
        errx(EXIT_FAILURE, "%s", SDL_GetError());

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, 
    SDL_RENDERER_ACCELERATED);

    if (renderer == NULL)
    {
        errx(EXIT_FAILURE, "%s", SDL_GetError());
    }


	SDL_Surface *surface_tmp = IMG_Load(filename);
	if (surface_tmp == NULL)
    {
        errx(EXIT_FAILURE, "%s", SDL_GetError());
    }

	SDL_Surface *surface = SDL_ConvertSurfaceFormat(surface_tmp, 
										SDL_PIXELFORMAT_RGB888, 0);
	if (surface == NULL)
    {
	    errx(EXIT_FAILURE, "%s", SDL_GetError());
    }

	SDL_FreeSurface(surface_tmp);

	SDL_SetWindowSize(window, surface->w,surface->h);

    SDL_Texture * texture = SDL_CreateTextureFromSurface(renderer,surface);
    if (texture == NULL)
    {
        errx(EXIT_FAILURE, "%s", SDL_GetError());
    }
	SDL_FreeSurface(surface);
	/// SDL STUFF END -------------------------------
	
	/// IMAGE AND LINES PRINTER
	int w,h;
	SDL_QueryTexture(texture, NULL, NULL, &w, &h);
	SDL_Rect rec_dest = {0,0,w,h};
	SDL_Event event;

	SDL_WaitEvent(&event);
	SDL_RenderClear(renderer);
	SDL_RenderCopy(renderer, texture, NULL, &rec_dest);
	SDL_SetRenderDrawColor(renderer, r, g, b, 0);
	for (st i = 0; i < nb_segments ; i++)
	{
		SDL_RenderDrawLine(renderer,segments[i]->x1,segments[i]->y1,
		segments[i]->x2,segments[i]->y2);
	}
	SDL_RenderPresent(renderer);
	/// IMAGE AND LINES PRINTER --------------------

	/// KEEP DISPLAY RESULT
	int keepDisplay = 1;
	while(keepDisplay)
	{
		SDL_WaitEvent(&event);
		switch (event.type)
		{
			case SDL_QUIT:
				keepDisplay = 0;
				break;
		}
	}
	/// KEEP DISPLAY RESULT--------------------------
	    
	/// DESTRUCTION----------------------------------

    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    IMG_Quit();
    SDL_Quit();
	return;
}

SDL_Surface* imageToSurface(Image *image)
{
    st w = image->width;
    st h = image->height;
    SDL_Surface *surface = SDL_CreateRGBSurfaceWithFormat(0, w, h, 8, SDL_PIXELFORMAT_RGB888);
    for (st y = 0; y < h; y++)
    {
        for (st x = 0; x < w; x++)
        {
            Uint8 r, g, b;
            uc *pixel = (uc *)surface->pixels + y * surface->pitch + x * surface->format->BytesPerPixel;
            uc value = image->pixels[y * w + x];
            SDL_GetRGB(value, surface->format, &r, &g, &b);
            *pixel = SDL_MapRGB(surface->format, r, g, b);
        }
    }
    return surface ;
}

int displayImage(Image *image)
{
    //SDL THINGS-----------------------------------------
    // Initializes the SDL.
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
        errx(EXIT_FAILURE, "%s", SDL_GetError());

    // Creates a window.
    SDL_Window* window;
    window = SDL_CreateWindow("Image visualizer", 0, 0, 1, 1,
                    SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
    
    if (window == NULL)
        errx(EXIT_FAILURE, "%s", SDL_GetError());

    // Creates a renderer.
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, 
                    SDL_RENDERER_ACCELERATED);
    if (renderer == NULL)
        errx(EXIT_FAILURE, "%s", SDL_GetError());

    SDL_Surface *surface = imageToSurface(image);
    if (surface == NULL)
        errx(EXIT_FAILURE, "%s", SDL_GetError());

    // Resize the window according to the size of the image.
    SDL_SetWindowSize(window, surface->w,surface->h);

    // Create a texture from the colored surface.
    SDL_Texture * texture = SDL_CreateTextureFromSurface(renderer,surface);
    if (texture == NULL)
        errx(EXIT_FAILURE, "%s", SDL_GetError());
    SDL_FreeSurface(surface);

    
//MAIN-----------------------------------------------
    event_loop(renderer, texture);

//DESTRUCTION--------------------------------------
    SDL_DestroyTexture(texture);
    //SDL_DestroyTexture(grayscale);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    IMG_Quit();
    SDL_Quit();

    return EXIT_SUCCESS;
}

//catch all events in an endless loop
void event_loop(SDL_Renderer* renderer, SDL_Texture *texture)
{
    SDL_Event event;
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
                        draw(renderer,texture);
                    }
                break;

            case SDL_KEYDOWN:
                if (event.key.keysym.scancode == SDL_SCANCODE_ESCAPE)
                    return;
                break;
        }

    }
}

void draw(SDL_Renderer* renderer, SDL_Texture * texture)
{ 
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, texture,NULL, NULL);
    SDL_RenderPresent(renderer);
}

