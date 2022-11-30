#include "viewexec.h"
#include "background.h"

ViewExec::ViewExec(Rectangle &limits, ViewRender *rnd, PaletteGroup *pals, ViewEventManager *evt, View *parent) : ViewGroup(limits, rnd, pals, parent), evtM(evt), background(nullptr)
{
    clearOptions(VIEW_OPT_ALL);
    setState(VIEW_STATE_VISIBLE | VIEW_STATE_SELECTED | VIEW_STATE_EXPOSED);
}

void ViewExec::initDesktop()
{
    Rectangle rect;
    getExtent(rect);
    background = new Background(rect);
    background->clearResizeMode(VIEW_RESIZEABLE | VIEW_ZOOMED);
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

    draw();
    while (getState(VIEW_STATE_EVLOOP))
    {
        while (evtM->wait(&event, 1000))
            handleEvent(&event);
    }
    clearState(VIEW_STATE_EVLOOP);
}

void ViewExec::draw()
{
    if (getState(VIEW_STATE_EVLOOP))
    {
        getRenderer()->clear(0);
        computeExposure();
        ViewGroup::draw();
        getRenderer()->show();
    }
}

void ViewExec::reDraw()
{
    if (getState(VIEW_STATE_EVLOOP))
    {
        getRenderer()->clear(0);
        computeExposure();
        ViewGroup::reDraw();
        getRenderer()->show();
    }
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

void ViewExec::handleEvent(Event *evt)
{
    ViewGroup::handleEvent(evt);

    if (evt->isEventKey())
    {
        KeyDownEvent *key = evt->getKeyDownEvent();
        // switch (key->keyCode)
        //{
        //         }
    }
}