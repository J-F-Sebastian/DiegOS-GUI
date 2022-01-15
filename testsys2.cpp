#include <iostream>

#include "SDL.h"
//#include "frame.h"
#include "button.h"
#include "progressbar.h"
#include "window.h"
#include "viewexec.h"
#include "eventqueue.h"
#include "viewrenderfactory.h"
#include "vieweventfactory.h"

//Screen dimension constants
const int SCREEN_WIDTH = 1024;
const int SCREEN_HEIGHT = 768;

// Frame Palette
const unsigned framePalette[] = {0x00FF00, 0x204220, 0x158F15};
const unsigned titlebarPalette[] = {0xF0F0F0, 0x000000};
const unsigned backgroundPalette[] = {0x0000A0};
const unsigned progressbarPalette[] = {0xFFFFFF, 0x123456, 0x654321};
const unsigned desktopPalette[] = {0x000015};
const unsigned buttonPalette[] = {0x00FF00, 0x204220, 0x158F15, 0xFFFFFF, 0x111111, 0x123456};

Palette *pal[PaletteGroup::PAL_NUM];

int main(int argc, char *argv[])
{
    SDL_Init(0);

    memset(pal, 0, sizeof(pal));
    pal[PaletteGroup::PAL_BACKGROUND] = new PaletteTrueColor(1);
    pal[PaletteGroup::PAL_BACKGROUND]->loadPalette(backgroundPalette);
    pal[PaletteGroup::PAL_FRAME] = new PaletteTrueColor(3);
    pal[PaletteGroup::PAL_FRAME]->loadPalette(framePalette);
    pal[PaletteGroup::PAL_TITLEBAR] = new PaletteTrueColor(2);
    pal[PaletteGroup::PAL_TITLEBAR]->loadPalette(titlebarPalette);
    pal[PaletteGroup::PAL_PROGRESSBAR] = new PaletteTrueColor(3);
    pal[PaletteGroup::PAL_PROGRESSBAR]->loadPalette(progressbarPalette);
    pal[PaletteGroup::PAL_DESKTOP] = new PaletteTrueColor(1);
    pal[PaletteGroup::PAL_DESKTOP]->loadPalette(desktopPalette);
    pal[PaletteGroup::PAL_BUTTON] = new PaletteTrueColor(6);
    pal[PaletteGroup::PAL_BUTTON]->loadPalette(buttonPalette);

    PaletteGroup *palg = new PaletteGroup(pal, PaletteGroup::PAL_NUM);

    Rectangle master(0, 0, SCREEN_WIDTH - 1, SCREEN_HEIGHT - 1);
    ViewRender *vr = ViewRenderFactory::create(VRENDER_HW, SCREEN_WIDTH, SCREEN_HEIGHT, 32);
    ViewEventManager *he = ViewEventFactory::create(EST_SDL);

    //ViewGroup app(master, vr, palg);
    ViewExec app(master, vr, palg);
    app.initDesktop();

    Rectangle a(100, 100, 400, 300);
    Window *all = new Window(a, "A Window", vr, palg, &app);
    app.insert(all);

    Rectangle aa(300, 400, 600, 500);
    View *butt = new Button(aa);
    butt->setPalette(palg->getPalette(PaletteGroup::PAL_BUTTON));
    app.insert(butt);
    Event event;
    app.handleEvent(&event);
    app.draw();

    while (he->wait(&event, 2000))
        ;

    Point b(100, 5);
    all->moveLocation(b);
    all->handleEvent(&event);
    app.draw();
    while (he->wait(&event, 2000))
        ;

    a.move(-50, 100);
    all->setLocation(a);
    all->handleEvent(&event);
    app.draw();
    while (he->wait(&event, 2000))
        ;

    a.zoom(-50, 100);
    std::cout << "Zoom" << std::endl;
    all->setLocation(a);
    all->handleEvent(&event);
    app.draw();
    while (he->wait(&event, 2000))
        ;

    b.set(-2, -1);
    for (unsigned i = 0; i < 20; i++)
    {
        all->moveLocation(b);
        all->handleEvent(&event);
        app.draw();
    }
    std::cout << "Ciao" << std::endl;

    delete vr;
    delete he;
    delete palg;
    SDL_Quit();
    return 0;
}