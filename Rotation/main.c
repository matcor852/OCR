#include "rotate_image.h"


int main(int argc, char ** argv)
{
    if (argc != 2)
    {
        errx(EXIT_FAILURE, "Invalid argues: input_file");
    }

    // Initializes the SDL.
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
        errx(EXIT_FAILURE, "%s", SDL_GetError());

    // Creates a window.
    SDL_Window* window = SDL_CreateWindow("Image editor", 0, 0, 1, 1,
            SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
    if (window == NULL)
        errx(EXIT_FAILURE, "%s", SDL_GetError());

    // Creates a renderer.
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == NULL)
        errx(EXIT_FAILURE, "%s", SDL_GetError());

    // - Create a surface from the image.
    SDL_Surface * surface = load_image(argv[1]);
    if (surface == NULL)
        errx(EXIT_FAILURE, "%s", SDL_GetError());

    // - Resize the window according to the size of the image.
    SDL_SetWindowSize(window, surface->w,surface->h);


    // - Create a texture from the colored surface.
    SDL_Texture * colored = SDL_CreateTextureFromSurface(renderer,surface);
    if (colored == NULL)
        errx(EXIT_FAILURE, "%s", SDL_GetError());

/*  //Grayscaled surface making
    surface_to_grayscale(surface);
    SDL_Texture * grayscale = SDL_CreateTextureFromSurface(renderer,surface);
    if (grayscale == NULL)
        errx(EXIT_FAILURE, "%s", SDL_GetError());
    SDL_FreeSurface(surface)
*/
    
    event_loop(renderer,colored);

    // Destruction
    SDL_DestroyTexture(colored);
    //SDL_DestroyTexture(grayscale);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return EXIT_SUCCESS;
}

//catch all events in an endless loop
void event_loop(SDL_Renderer* renderer, SDL_Texture* colored)
{
    SDL_Event event;
    SDL_Texture* t = colored;
    // Width and height of the window.
    int w,h;
    // Variable of angle of editing
    float angle = 0;

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
                        draw(renderer,t,angle);
                    }
                break;

            case SDL_KEYDOWN:
                if (event.key.keysym.scancode == SDL_SCANCODE_S)
                    save_texture("edited.png",renderer,t,w,h);
                if (event.key.keysym.scancode == SDL_SCANCODE_LEFT || event.key.keysym.scancode == SDL_SCANCODE_A)
                    angle = angle - 0.2;
                if (event.key.keysym.scancode == SDL_SCANCODE_RIGHT || event.key.keysym.scancode == SDL_SCANCODE_D)
                    angle = angle + 0.2;
                if (event.key.keysym.scancode == SDL_SCANCODE_ESCAPE)
                    return;
                draw(renderer,t,angle);
                break;
        }

    }
}