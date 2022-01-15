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

#include "frame.h"

Frame::Frame(Rectangle &rect) : View(rect)
{
    setResizeMode(VIEW_RESIZE_LX | VIEW_RESIZE_LY);
    clearOptions(VIEW_OPT_TOPSELECT);
}

void Frame::draw()
{
    Rectangle viewRect;
    getExtent(viewRect);
    globalize(viewRect);
    //std::cout << __PRETTY_FUNCTION__ << "(" << viewRect.ul.x << "," << viewRect.ul.y << ")x("
    //          << viewRect.lr.x << "," << viewRect.lr.y << ")" << std::endl;
    unsigned color, color2;

    Rectangle temp(viewRect);

    palette->getPalette(FRAME_BRIGHT, color);
    palette->getPalette(FRAME_DARK, color2);

    //Outer shadow

    /*
     *    BBBBBBBBB
     *    B       D
     *    B       D
     *    B       D
     *    DDDDDDDDD
     */
    Point ul(temp.ul);
    renderer->hline(ul, temp.width(), color);
    renderer->vline(ul, temp.height() - 1, color);
    ul.move(temp.width(), 1);
    renderer->vline(ul, temp.height() - 1, color2);
    ul.move(-temp.width(), temp.height() - 1);
    renderer->hline(ul, temp.width() - 1, color2);

    //The frame
    temp.zoom(-1, -1);
    palette->getPalette(FRAME_MAIN, color);
    renderer->rectangle(temp, color);

    //Inner shadow

    /*
     *    DDDDDDDDD
     *    D       B
     *    D       B
     *    D       B
     *    BBBBBBBBB
     */
    temp.zoom(-1, -1);
    palette->getPalette(FRAME_BRIGHT, color);
    ul = temp.ul;
    renderer->hline(ul, temp.width(), color2);
    renderer->vline(ul, temp.height() - 1, color2);
    ul.move(temp.width(), 1);
    renderer->vline(ul, temp.height() - 1, color);
    ul.move(-temp.width(), temp.height() - 1);
    renderer->hline(ul, temp.width() - 1, color);
}

void Frame::handleEvent(Event *evt)
{
    View::handleEvent(evt);
    if (evt)
    {
        Rectangle viewRect;
        getExtent(viewRect);
        std::cout << "EVT FRAME (" << viewRect.ul.x << "," << viewRect.ul.y << ")x("
                  << viewRect.lr.x << "," << viewRect.lr.y << ")" << std::endl;
    }
}

bool Frame::isEventPositionValid(Event *evt)
{
    Rectangle lims;

    if (!evt)
        return false;

    /* If the event is not positional return false */
    if (!evt->isEventPositional())
        return false;

    MouseEvent *mouse = evt->getMouseEvent();
    Point where(mouse->x, mouse->y);
    getExtent(lims);
    globalize(lims);
    /* if it falls outside our limits then return false */
    bool inside = lims.includes(where);
    if (inside)
    {
        lims.zoom(-3, -3);
        return !lims.includes(where);
    }
    return false;
}