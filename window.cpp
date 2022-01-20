/*
 * DiegOS Operating System source code
 *
 * Copyright (C) 2012 - 2019 Diego Gallizioli
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <iostream>

#include "window.h"
#include "window_icon.h"
#include "frame.h"
#include "titlebar.h"
#include "background.h"

Window::Window(Rectangle &viewLimits, const char *title, ViewRender *rnd, PaletteGroup *pals, View *parent, unsigned char ctrlflags) : ViewGroup(viewLimits, rnd, pals, parent), wFlags(ctrlflags), isZoomed(false)
{
    setOptions(VIEW_OPT_TOPSELECT | VIEW_OPT_TILEABLE | VIEW_OPT_SELECTABLE);

    Rectangle temp;
    getExtent(temp);
    View *tmpView = new Frame(temp);
    tmpView->setPalette(palettes->getPalette(PaletteGroup::PAL_FRAME));
    insert(tmpView);

    temp.zoom(-4, -4);
    temp.ul.y += 26;
    tmpView = new Background(temp);
    tmpView->setPalette(palettes->getPalette(PaletteGroup::PAL_BACKGROUND));
    insert(tmpView);

    getExtent(temp);
    temp.zoom(-4, -4);
    temp.lr.y = temp.ul.y + 24;
    if (wFlags & WINDOW_CLOSE)
    {
        Rectangle iconRect(temp);
        iconRect.lr.x = iconRect.ul.x + 24;
        temp.ul.x += 25;
        tmpView = new WindowIconClose(iconRect);
        tmpView->setPalette(palettes->getPalette(PaletteGroup::PAL_WINICON));
        insert(tmpView);
    }
    if (wFlags & WINDOW_ZOOM)
    {
        Rectangle iconRect(temp);
        iconRect.ul.x = iconRect.lr.x - 24;
        temp.lr.x -= 25;
        tmpView = new WindowIconZoom(iconRect);
        tmpView->setPalette(palettes->getPalette(PaletteGroup::PAL_WINICON));
        insert(tmpView);
    }
    tmpView = new TitleBar(temp, title);
    tmpView->setPalette(palettes->getPalette(PaletteGroup::PAL_TITLEBAR));
    insert(tmpView);
}