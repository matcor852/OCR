#include "main.h"

int rotate(int argc, char ** argv)
{
//ARGUMENTS ANALYSIS---------------------------------
    if (argc != 4)
        errx(EXIT_FAILURE, "%s", "Arguments must be : filename --rotate angle or --rotate -p");

    int preview = 0; //if mode preview is wanted (with flag -p)
    for (size_t i = 0; argv[3][i]!=0;i++)
    {
        if ( argv[3][i]!='p' && argv[3][i]!='-' && 
        (argv[3][i] > 57 || argv[3][i] <48) )
            errx(EXIT_FAILURE, "%s", "Argument 3 must be \"-p\" or an angle");
        if (argv[3][i]=='-')
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
    if(preview)
        event_loop(renderer,colored);
    else
    {
        //angle entered in arg
        double angle = strtoul(argv[3],NULL,10);
        //get width and height of the orignal image
        int w,h;
        SDL_QueryTexture(colored, NULL, NULL, &w, &h);
        SDL_Rect rec_dest = {0,0,w,h};

        draw(renderer,colored,angle);
        save_renderer("edited.png",renderer,rec_dest);
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

//catch all events in an endless loop
void event_loop(SDL_Renderer* renderer, SDL_Texture* colored)
{
    SDL_Event event;
    // Width and height of the window.
    SDL_Rect rec_dest = {0,0,0,0};
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
                        rec_dest.w = event.window.data1;
                        rec_dest.h = event.window.data2;
                        draw(renderer,colored,angle);
                    }
                break;

            case SDL_KEYDOWN:
                if (event.key.keysym.scancode == SDL_SCANCODE_S)
                    save_renderer("edited.png",renderer,rec_dest);

                if (event.key.keysym.scancode == SDL_SCANCODE_LEFT || event.key.keysym.scancode == SDL_SCANCODE_A)
                    {
                        angle = angle - step;
                        draw(renderer,colored,angle);
                    }

                if (event.key.keysym.scancode == SDL_SCANCODE_RIGHT || event.key.keysym.scancode == SDL_SCANCODE_D)
                    {
                        angle = angle + step;
                        draw(renderer,colored,angle);
                    }

                if (event.key.keysym.scancode == SDL_SCANCODE_ESCAPE)
                    return;
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

//it saves a specific rectangle of a renderer
void save_renderer(const char* file_name, SDL_Renderer* renderer, SDL_Rect rec_dest)
{
    SDL_Surface* surface = SDL_CreateRGBSurface(0, rec_dest.w, rec_dest.h, 32, 0, 0, 0, 0);
    if (surface == NULL)
        errx(EXIT_FAILURE, "%s", SDL_GetError());
    
    //copy each pixel of renderer in a surface
    SDL_RenderReadPixels(renderer, &rec_dest, surface->format->format, surface->pixels, surface->pitch);

    //save surface in file_name format png
    IMG_SavePNG(surface, file_name);

    SDL_FreeSurface(surface);
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

