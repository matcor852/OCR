#include "main.h"

int resize(int argc, char ** argv)
{
//ARGUMENTS ANALYSIS---------------------------------
    if (argc != 8)
        errx(EXIT_FAILURE, "%s", "Arguments must be : filename --resize x y width height newSize");
    
    for (size_t j = 3; j < 8; j++)
    {
        for (size_t i = 0; argv[j][i]!=0;i++)
        {
            if (argv[j][i] > 57 || argv[j][i] <48)
                errx(EXIT_FAILURE, "%s", "Wrong argument type");
        }
    }

//SDL THINGS-----------------------------------------
    // Initializes the SDL.
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
        errx(EXIT_FAILURE, "%s", SDL_GetError());

    // Creates a window.
    SDL_Window* window;
    window = SDL_CreateWindow("Resizor", 0, 0, 1, 1,
                    SDL_WINDOW_HIDDEN | SDL_WINDOW_RESIZABLE);
    
    if (window == NULL)
        errx(EXIT_FAILURE, "%s", SDL_GetError());

    // Creates a renderer.
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, 
                    SDL_RENDERER_ACCELERATED);
    if (renderer == NULL)
        errx(EXIT_FAILURE, "%s", SDL_GetError());

    // Create a surface from the image.
    SDL_Surface * surface = load_image(argv[1]);
    if (surface == NULL)
        errx(EXIT_FAILURE, "%s", SDL_GetError());

    // Resize the window according to the size of the image.
    SDL_SetWindowSize(window, surface->w,surface->h);

    // Create a texture from the colored surface.
    SDL_Texture * colored = SDL_CreateTextureFromSurface(renderer,surface);
    if (colored == NULL)
        errx(EXIT_FAILURE, "%s", SDL_GetError());

    SDL_FreeSurface(surface);
    
//MAIN-----------------------------------------------

    int x_scr = strtoul(argv[3],NULL,10);
    int y_scr = strtoul(argv[4],NULL,10);
    int w_scr = strtoul(argv[5],NULL,10);
    int h_scr = strtoul(argv[6],NULL,10);
    //Source rectangle : pos and size of source rectangle
    SDL_Rect rec_src = {x_scr, y_scr, w_scr, h_scr};

    int size_dest = strtoul(argv[7],NULL,10);;
    //Destination rectangle : size of image that will be created
    SDL_Rect rec_dest = {0,0,size_dest,size_dest};

    SDL_RenderCopy(renderer, colored, &rec_src, &rec_dest);
    SDL_RenderPresent(renderer);
    save_renderer("edited.png",renderer,rec_dest);

    
//DESTRUCTION--------------------------------------
    SDL_DestroyTexture(colored);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    IMG_Quit();
    SDL_Quit();

    return EXIT_SUCCESS;
}