#include <iostream>

#define SDL_MAIN_HANDLED
#include "SDL.h"
// #include "frame.h"
// #include "eventqueue.h"

// Screen dimension constants
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

// The window we'll be rendering to
SDL_Window *window = NULL;

// The renderer
SDL_Renderer *renderer = NULL;

// Frame Palette
const unsigned framePalette[] = {0xff0000, 0x0000ff, 0x00ff00};

int main(int argc, char *argv[])
{
    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        std::cout << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
    }
    else
    {
        // Create window
        if (SDL_CreateWindowAndRenderer(640, 480, 0, &window, &renderer))
        {
            std::cout << "Window could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        }
        else
        {
            SDL_RenderClear(renderer);
            // Fill the surface white
            SDL_SetRenderDrawColor(renderer, 0xFF, 0x00, 0xFF, SDL_ALPHA_OPAQUE);
            // SDL_Rect rect = {.x = 0, .y = 0, .w = 640, .h = 480};
            SDL_RenderFillRect(renderer, NULL);

            // Update the surface
            // SDL_RenderPresent(renderer);
            // Wait two seconds
            // SDL_Delay(2000);
#if 0
            Rectangle a(1, 1, 200, 200);
            View *b = new Frame(a);
            Palette *pb = new PaletteTrueColor(3);
            pb->loadPalette(framePalette);
            b->setPalette(pb);
            Rectangle c(300, 300, 400, 400);
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
#endif

            SDL_Texture *texture = SDL_CreateTexture(renderer,
                                                     SDL_PIXELFORMAT_ARGB8888,
                                                     SDL_TEXTUREACCESS_TARGET,
                                                     100,
                                                     100);

            SDL_SetRenderTarget(renderer, texture);

            SDL_Texture *texture2 = SDL_CreateTexture(renderer,
                                                      SDL_PIXELFORMAT_ARGB8888,
                                                      SDL_TEXTUREACCESS_TARGET,
                                                      150,
                                                      100);

            SDL_Texture *texture3 = SDL_CreateTexture(renderer,
                                                      SDL_PIXELFORMAT_ARGB8888,
                                                      SDL_TEXTUREACCESS_TARGET,
                                                      100,
                                                      150);

            SDL_Rect src, dst;

            src = {0, 0, 100, 100};
            dst = {1, 1, 100, 100};

            SDL_SetRenderTarget(renderer, texture);
            SDL_SetRenderDrawColor(renderer, 0xFF, 0x00, 0x00, SDL_ALPHA_OPAQUE);
            SDL_RenderFillRect(renderer, NULL);
            SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, SDL_ALPHA_OPAQUE);
            SDL_RenderDrawLine(renderer, 1, 1, 99, 99);
            SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, SDL_ALPHA_OPAQUE);
            SDL_RenderDrawLine(renderer, 99, 1, 1, 99);
            // SDL_SetRenderTarget(renderer, NULL);
            // SDL_RenderCopy(renderer, texture, &src, &dst);

            SDL_SetRenderTarget(renderer, texture2);
            SDL_SetRenderDrawColor(renderer, 0x00, 0xFF, 0x00, SDL_ALPHA_OPAQUE);
            SDL_RenderFillRect(renderer, NULL);
            SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, SDL_ALPHA_OPAQUE);
            SDL_RenderDrawLine(renderer, 1, 1, 99, 99);
            SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, SDL_ALPHA_OPAQUE);
            SDL_RenderDrawLine(renderer, 99, 1, 1, 99);
            // SDL_SetRenderTarget(renderer, NULL);
            // dst.x = dst.y = 101;
            // SDL_RenderCopy(renderer, texture2, &src, &dst);

            SDL_SetRenderTarget(renderer, texture3);
            SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0xFF, SDL_ALPHA_OPAQUE);
            SDL_RenderFillRect(renderer, NULL);
            SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, SDL_ALPHA_OPAQUE);
            SDL_RenderDrawLine(renderer, 1, 1, 99, 99);
            SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, SDL_ALPHA_OPAQUE);
            SDL_RenderDrawLine(renderer, 99, 1, 1, 99);

            SDL_SetRenderTarget(renderer, NULL);
            SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);
            SDL_RenderCopy(renderer, texture, &src, &dst);
            dst.x = dst.y = 101;
            // SDL_SetRenderTarget(renderer, NULL);
            SDL_RenderCopy(renderer, texture2, &src, &dst);
            dst.x = dst.y = 152;
            // SDL_SetRenderTarget(renderer, NULL);
            SDL_RenderCopy(renderer, texture3, NULL, &dst);

            // Update the surface
            SDL_RenderPresent(renderer);

            // Wait two seconds
            SDL_Delay(5000);

            SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, SDL_ALPHA_OPAQUE);
            // SDL_Rect rect = {.x = 0, .y = 0, .w = 640, .h = 480};
            SDL_RenderFillRect(renderer, NULL);

            SDL_SetRenderTarget(renderer, NULL);
            dst.x = dst.y = 1;
            SDL_RenderCopy(renderer, texture, NULL, &dst);
            dst.x = dst.y = 101;
            SDL_RenderCopy(renderer, texture2, NULL, &dst);
            dst.x = dst.y = 181;
            SDL_RenderCopy(renderer, texture3, NULL, &dst);

            // Update the surface
            SDL_RenderPresent(renderer);

            // Wait two seconds
            SDL_Delay(5000);

            SDL_DestroyTexture(texture);
            SDL_DestroyTexture(texture2);
            SDL_DestroyTexture(texture3);
        }

        // Destroy window
        SDL_DestroyWindow(window);

        // Quit SDL subsystems
        SDL_Quit();
    }

    return 0;
#if 0
    Rectangle a(1, 1, 2, 2);
    View *b = new Frame(a);
    Rectangle c(3, 3, 4, 4);
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

    for (int i = 10000; i; i--)
        queue.enqueue(new Event());

    std::cout << "Ciao" << std::endl;

    delete b;
    delete d;

   return 0;
#endif
}