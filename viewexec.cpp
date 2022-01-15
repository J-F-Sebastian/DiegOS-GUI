#include "viewexec.h"
#include "background.h"

ViewExec::ViewExec(Rectangle &limits, ViewRender *rnd, PaletteGroup *pals, View *parent) : ViewGroup(limits, rnd, pals, parent)
{
    clearOptions(VIEW_OPT_ALL);
}

void ViewExec::initDesktop()
{
    Rectangle rect;
    getExtent(rect);
    View *bg = new Background(rect);
    bg->clearResizeMode(VIEW_RESIZEABLE);
    bg->setPalette(palettes->getPalette(PaletteGroup::PAL_DESKTOP));
    insert(bg);
}

void ViewExec::initMenu()
{
}
