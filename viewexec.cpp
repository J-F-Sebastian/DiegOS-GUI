#include "viewexec.h"
#include "background.h"

ViewExec::ViewExec(Rectangle &limits, ViewRender *rnd, PaletteGroup *pals, ViewEventManager *evt, View *parent) : ViewGroup(limits, rnd, pals, parent), evtM(evt)
{
    clearOptions(VIEW_OPT_ALL);
}

void ViewExec::initDesktop()
{
    Rectangle rect;
    getExtent(rect);
    background = new Background(rect);
    background->clearOptions(VIEW_OPT_ALL);
    background->clearResizeMode(VIEW_RESIZEABLE);
    background->setPalette(palettes->getPalette(PaletteGroup::PAL_DESKTOP));
    insert(background);
}

void ViewExec::initMenu()
{
}

void ViewExec::run()
{
    setState(VIEW_STATE_EVLOOP);
    Event event;

    while (getState(VIEW_STATE_EVLOOP))
    {
        draw();
        if (evtM->wait(&event, 1000))
            handleEvent(&event);
    }
    clearState(VIEW_STATE_EVLOOP);
}

void ViewExec::sendEvent(Event *evt)
{
    if (getState(VIEW_STATE_EVLOOP))
    {
        evtM->put(evt);
    }
    else
        View::sendEvent(evt);
}