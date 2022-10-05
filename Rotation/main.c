#include "rotate_image.h"


int main(int argc, char ** argv)
{
    //ARGUMENTS ANALYSIS---------------------------------
    if (argc != 3)
        errx(EXIT_FAILURE, "%s", "Not enough arguments");

    int preview = 0; //if mode preview is wanted (with flag -p)
    for (size_t i = 0; argv[2][i]!=0;i++)
    {
        if ( (argv[2][i]!='p' && argv[2][i]!='-') && 
        (argv[2][i] > 57 || argv[2][i] <48) )
            errx(EXIT_FAILURE, "%s", "Argument 2 must be \"-p\" or an angle");
        if (argv[2][i]=='-')
            preview = 1;
    }


    //SDL THINGS-----------------------------------------
    // Initializes the SDL.
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
        errx(EXIT_FAILURE, "%s", SDL_GetError());

    // Creates a window.
    SDL_Window* window;
    if (preview)
        window = SDL_CreateWindow("Image editor", 0, 0, 1, 1,
                    SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
    else
        window = SDL_CreateWindow("Image editor", 0, 0, 1, 1,
                    SDL_WINDOW_HIDDEN | SDL_WINDOW_RESIZABLE);
    if (window == NULL)
        errx(EXIT_FAILURE, "%s", SDL_GetError());

    // Creates a renderer.
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, 
                    SDL_RENDERER_ACCELERATED);
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
    /*
    //Grayscaled surface making
    surface_to_grayscale(surface);
    SDL_Texture * grayscale = SDL_CreateTextureFromSurface(renderer,surface);
    if (grayscale == NULL)
        errx(EXIT_FAILURE, "%s", SDL_GetError());
    */
    SDL_FreeSurface(surface);

    
    //MAIN-----------------------------------------------
    if(preview)
        event_loop(renderer,colored);
    else
    {
        double angle = strtoul(argv[2],NULL,10);
        int w,h;
        SDL_QueryTexture(colored, NULL, NULL, &w, &h);
        int end = 3;
        while (end>1)
        {
            SDL_RenderCopyEx(renderer,colored, NULL, NULL,angle,NULL,0);
            SDL_RenderPresent(renderer);
            save_texture("edited.png",renderer,colored,w,h);
            end = end-1;
        }
    }  

    //DESTRUCTION--------------------------------------
    SDL_DestroyTexture(colored);
    //SDL_DestroyTexture(grayscale);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    IMG_Quit();
    SDL_Quit();

    return EXIT_SUCCESS;
}