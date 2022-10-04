#include <stdio.h>
#include <err.h>
#include <SDL2/SDL.h>

#include <SDL2/SDL_image.h>

#define MIN(a, b) ( ( (a) < (b) ) ? (a) : (b) )
#define MAX(a, b) ( ( (a) > (b) ) ? (a) : (b) )
#define DIM(x, a, b) ( MIN(MAX(x, a), b) )
#define 	M_PI   3.141592654
#define 	SMOOTHING_OFF   0
#define 	SMOOTHING_ON   1
#define 	SDL_ROTOZOOM_SCOPE   extern

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

void draw(SDL_Renderer* renderer, SDL_Texture * texture, int mouse_x, int mouse_y)
{ 


    SDL_RenderClear(renderer);

    double angle = ((mouse_x+mouse_y)/2) % 3141;
    SDL_RenderCopyEx(renderer, texture , NULL, NULL, angle, NULL, 0);
    SDL_RenderPresent(renderer);
}


// Event loop that calls the relevant event handler.
//
// renderer: Renderer to draw on.
// colored: Texture that contains the colored image.
// grayscale: Texture that contains the grayscale image.
void event_loop(SDL_Renderer* renderer, SDL_Texture* colored, SDL_Texture* grayscale)
{
    SDL_Event event;
    SDL_Texture* t = colored;

    // Width and height of the window.


    // (Fake) position of the mouse cursor.
    int mouse_x = 100;
    int mouse_y = 100;
    int w;
    int h;

    draw(renderer, colored,mouse_x,mouse_y);

    while (1)
    {
        // Waits for an event.
        SDL_WaitEvent(&event);

        switch (event.type)
        {
            // If the "quit" button is pushed, ends the event loop.
            case SDL_QUIT:	
                return;
            // If the window is resized, updates and redraws the diagonals.
            case SDL_WINDOWEVENT:
                if (event.window.event == SDL_WINDOWEVENT_RESIZED)
		{
			w = event.window.data1;
			h = event.window.data2;
                        draw(renderer,t,mouse_x,mouse_y);
		}
                break;
            case SDL_KEYDOWN:
                if (t == colored)
                    t = grayscale;
                else
                    t = colored;
                draw(renderer,t,mouse_x,mouse_y);
		if (event.key.keysym.scancode == SDL_SCANCODE_S)
			save_texture("edited.png",renderer,t,w,h);
		break;
	
            // If the mouse is moving, updates the position of the cursor.
            case SDL_MOUSEMOTION:
                mouse_x = event.motion.x;
		mouse_y = event.motion.y;
                draw(renderer,t, mouse_x,mouse_y);
                break;
	
        }

    }
}

// Loads an image in a surface.
// The format of the surface is SDL_PIXELFORMAT_RGB888.
//
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
//
// pixel_color: Color of the pixel to convert in the RGB format.
// format: Format of the pixel used by the surface.
Uint32 pixel_to_grayscale(Uint32 pixel_color, SDL_PixelFormat* format)
{
    Uint8 r, g, b;

    SDL_GetRGB(pixel_color, format, &r, &g, &b);

    Uint8 avrg = 0.3*r + 0.59*g + 0.11*b;
    Uint32 color = SDL_MapRGB(format, avrg , avrg , avrg );

    return color;
}

void surface_to_grayscale(SDL_Surface* surface)
{
    Uint32* pixels = surface->pixels;
    int w = surface->w;
    int h = surface->h;
    SDL_PixelFormat* format = surface->format;

    SDL_LockSurface(surface);
    for (int height = 0; height < h; height++)

    {
	for (int width = 0; width < w; width++)
	{
		pixels[width + height*w] = pixel_to_grayscale(pixels[width + height*w],format);

	}
    }
    SDL_UnlockSurface(surface);
}


int main()
{
    // Initializes the SDL.
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
        errx(EXIT_FAILURE, "%s", SDL_GetError());
    // Creates a window.
    SDL_Window* window = SDL_CreateWindow("Image", 0, 0, 1, 1,
            SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
    if (window == NULL)
        errx(EXIT_FAILURE, "%s", SDL_GetError());
    // Creates a renderer.
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == NULL)
        errx(EXIT_FAILURE, "%s", SDL_GetError());

    // - Create a surface from the image.
    SDL_Surface * surface = load_image("Images/image_04.jpeg");

    if (surface == NULL)
        errx(EXIT_FAILURE, "%s", SDL_GetError());

    // - Resize the window according to the size of the image.
    SDL_SetWindowSize(window, surface->w,surface->h);

    // - Create a texture from the colored surface.
    SDL_Texture * colored = SDL_CreateTextureFromSurface(renderer,surface);
    if (colored == NULL)
        errx(EXIT_FAILURE, "%s", SDL_GetError());

    // - Convert the surface into grayscale.
    surface_to_grayscale(surface);

    // - Create a new texture from the grayscale surface.
    SDL_Texture * grayscale = SDL_CreateTextureFromSurface(renderer,surface);

    if (grayscale == NULL)
        errx(EXIT_FAILURE, "%s", SDL_GetError());

    // - Free the surface.
    SDL_FreeSurface(surface);

    // Dispatches the events.
    event_loop(renderer,colored,grayscale);

    // Destroys the objects.
    SDL_DestroyTexture(colored);
    SDL_DestroyTexture(grayscale);

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return EXIT_SUCCESS;
}




/*
int main()
{

	char *filename = "Images/image_01.jpeg";
	st width, height;
	uchar **pixels = openImage(filename, &width, &height);
	printImage(width, height, pixels);
	return 0;
}*/
