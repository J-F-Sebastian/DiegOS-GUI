#include <iostream>

#include "SDL.h"
#include "frame.h"
#include "eventqueue.h"

//Screen dimension constants
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

//The window we'll be rendering to
SDL_Window* window = NULL;

//The renderer
SDL_Renderer* renderer = NULL;

// Frame Palette
const unsigned framePalette[] = {0xff0000, 0x0000ff, 0x00ff00};

int main(int argc, char *argv[])
{
    //Initialize SDL
    if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
    {
        std::cout << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
    }
    else
    {
        //Create window
        if (SDL_CreateWindowAndRenderer(640, 480, 0, &window, &renderer))
        {
            std::cout << "Window could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        }
        else
        {
            SDL_RenderClear(renderer);
            //Fill the surface white
 //           SDL_SetRenderDrawColor(renderer, 0xFF, 0x00, 0xFF, SDL_ALPHA_OPAQUE);
 //           SDL_Rect rect = {.x = 1, .y = 1, .w = 638, .h = 478};
 //           SDL_RenderFillRect(renderer, &rect);
    Rectangle a(1,1,200,200);
    View *b = new Frame(a);
    Palette *pb = new PaletteTrueColor(3);
    pb->loadPalette(framePalette);
    b->setPalette(pb);
    Rectangle c(300,300,400,400);
    View *d = new Frame(c);
    ViewGroup all;
    Event event;

    all.insert(b);
    all.insert(d);
    all.handleEvent(&event);
    all.draw();

    View *aa = new Frame(a);
    all.insertFront(aa);
    View *bb = new Frame(c);
    all.insert(bb);

    all.handleEvent(&event);
    all.draw();
            //Update the surface
            SDL_RenderPresent(renderer);

            //Wait two seconds
            SDL_Delay( 2000 );
        }

        //Destroy window
        SDL_DestroyWindow( window );

        //Quit SDL subsystems
        SDL_Quit();
    }

    return 0;

    Rectangle a(1,1,2,2);
    View *b = new Frame(a);
    Rectangle c(3,3,4,4);
    View *d = new Frame(c);
    ViewGroup all;
    Event event;

    all.insert(b);
    all.insert(d);
    all.handleEvent(&event);
    all.draw();

    View *aa = new Frame(a);
    all.insertFront(aa);
    View *bb = new Frame(c);
    all.insert(bb);

    all.handleEvent(&event);
    all.draw();

    EventQueue queue;

    for (int i = 10000; i; i--) queue.enqueue(new Event());

    std::cout << "Ciao" << std::endl;
    
    delete b;
    delete d;
    return 0;
}