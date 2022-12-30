#include "viewexec.h"
#include "background.h"
#include "event_keyboard.h"

ViewExec::ViewExec(Rectangle &limits, ViewRender *rnd, PaletteGroup *pals, ViewEventManager *evt, View *parent) : ViewGroup(limits, rnd, pals, parent), evtM(evt)
{
    clearOptions(VIEW_OPT_ALL);
    setState(VIEW_STATE_VISIBLE | VIEW_STATE_SELECTED | VIEW_STATE_EXPOSED);
}

void ViewExec::run()
{
    Event event;

    setState(VIEW_STATE_EVLOOP);
    sendCommand(CMD_REDRAW);

    while (getState(VIEW_STATE_EVLOOP))
    {
        while (evtM->wait(&event, 1000))
            handleEvent(&event);
    }
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
        ViewGroup::reDraw();
        getRenderer()->show();
    }
}

void ViewExec::sendEvent(Event *evt)
{
    std::cout << __FUNCTION__ << std::endl;
    if (getState(VIEW_STATE_EVLOOP))
    {
        evtM->put(evt);
    }
    else
        View::sendEvent(evt);
}

void ViewExec::handleEvent(Event *evt)
{
    if (evt->isEventKey())
    {
        KeyDownEvent *key = evt->getKeyDownEvent();
        switch (key->keyCode)
        {
        case KBD_CODE_F4:
            if (key->modifier & KBD_MOD_ALT)
            {
                // ViewGroup will take care of this
                sendCommand(CMD_CLOSE, this, actual);
                evt->clear();
                return;
            }
            break;
        }
    }

    ViewGroup::handleEvent(evt);
}