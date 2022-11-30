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

#include "background.h"
#include "background_palette.h"

Background::Background(Rectangle &rect) : View(rect)
{
    setResizeMode(VIEW_RESIZEABLE);
}

void Background::draw()
{
    Rectangle viewRect;
    ViewRender *renderer = getRenderer();
    Palette *palette = getPalette();
    getExtent(viewRect);
    globalize(viewRect);
    unsigned color;

    if (getState(VIEW_STATE_DISABLED))
        palette->getPalette(BACKGROUND_BG_DISABLED, color);
    else
        palette->getPalette(BACKGROUND_BG, color);

    renderer->filledRectangle(viewRect, color);
}

void Background::handleEvent(Event *evt)
{
    View::handleEvent(evt);
}