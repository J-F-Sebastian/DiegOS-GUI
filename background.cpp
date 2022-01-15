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

Background::Background(Rectangle &rect) : View(rect)
{
    clearOptions(VIEW_OPT_SELECTABLE | VIEW_OPT_TOPSELECT);
    clearResizeMode(VIEW_RESIZEABLE);
}

void Background::draw()
{
    Rectangle viewRect;
    getExtent(viewRect);
    globalize(viewRect);
    //std::cout << __PRETTY_FUNCTION__ << "(" << viewRect.ul.x << "," << viewRect.ul.y << ")x("
    //<< viewRect.lr.x << "," << viewRect.lr.y << ")" << std::endl;
    unsigned color;

    palette->getPalette(BACKGROUND_COLOR, color);

    renderer->filledRectangle(viewRect, color);
}

void Background::handleEvent(Event *evt)
{
    View::handleEvent(evt);
    if (evt)
    {
        Rectangle viewRect;
        getExtent(viewRect);
        //std::cout << "EVT BG (" << viewRect.ul.x << "," << viewRect.ul.y << ")x("
        //        << viewRect.lr.x << "," << viewRect.lr.y << ")" << std::endl;
    }
}