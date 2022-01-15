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

#include "window_icon.h"

WindowIcon::WindowIcon(Rectangle &rect) : View(rect), pressState(RELEASED)
{
    clearOptions(VIEW_OPT_ALL);
}

void WindowIconClose::draw()
{
    Rectangle viewRect;
    getExtent(viewRect);
    globalize(viewRect);
    Rectangle temp(viewRect);
    //std::cout << __PRETTY_FUNCTION__ << "(" << viewRect.ul.x << "," << viewRect.ul.y << ")x("
    //          << viewRect.lr.x << "," << viewRect.lr.y << ")" << std::endl;

    unsigned color, color2;
    palette->getPalette(WINICON_BRIGHT, color);
    palette->getPalette(WINICON_DARK, color2);

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

    //The button frame
    palette->getPalette(WINICON_MAIN, color);
    temp.zoom(-1, -1);
    renderer->filledRectangle(temp, color);

    if (isPressed())
    {
        palette->getPalette(WINICON_PRESSED, color);
    }
    else
    {
        palette->getPalette(WINICON_RELEASED, color);
    }

    temp.zoom(-4, -4);
    Point s(temp.ul), e(temp.lr);
    renderer->line(s, e, color);
    s.x += 1;
    e.y -= 1;
    renderer->line(s, e, color);
    s.x += 1;
    e.y -= 1;
    renderer->line(s, e, color);
    s.x = temp.lr.x;
    s.y = temp.ul.y;
    e.x = temp.ul.x;
    e.y = temp.lr.y;
    renderer->line(s, e, color);
    s.x -= 1;
    e.y -= 1;
    renderer->line(s, e, color);
    s.x -= 1;
    e.y -= 1;
    renderer->line(s, e, color);
}

void WindowIconClose::handleEvent(Event *evt)
{
    View::handleEvent(evt);
    if (evt)
    {
        Rectangle viewRect;
        getExtent(viewRect);
        std::cout << "EVT ICON CLOSE (" << viewRect.ul.x << "," << viewRect.ul.y << ")x("
                  << viewRect.lr.x << "," << viewRect.lr.y << ")" << std::endl;
    }
}

void WindowIconZoom::draw()
{
    Rectangle viewRect;
    getExtent(viewRect);
    globalize(viewRect);
    Rectangle temp(viewRect);
    //std::cout << __PRETTY_FUNCTION__ << "(" << viewRect.ul.x << "," << viewRect.ul.y << ")x("
    //          << viewRect.lr.x << "," << viewRect.lr.y << ")" << std::endl;

    unsigned color, color2;
    palette->getPalette(WINICON_BRIGHT, color);
    palette->getPalette(WINICON_DARK, color2);

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

    //The button frame
    palette->getPalette(WINICON_MAIN, color);
    temp.zoom(-1, -1);
    renderer->filledRectangle(temp, color);

    if (isPressed())
    {
        palette->getPalette(WINICON_PRESSED, color);
    }
    else
    {
        palette->getPalette(WINICON_RELEASED, color);
    }

    if (isZoom)
    {
        temp.zoom(-3, -3);
        renderer->rectangle(temp, color);
        temp.zoom(-1, -1);
        renderer->rectangle(temp, color);
        temp.zoom(-1, -1);
        renderer->hline(temp.ul, temp.width(), color);
    }
    else
    {
        temp.zoom(-6, -6);
        temp.move(-1, 1);
        renderer->rectangle(temp, color);
        temp.zoom(-1, -1);
        renderer->hline(temp.ul, temp.width(), color);
    }
}

void WindowIconZoom::handleEvent(Event *evt)
{
    View::handleEvent(evt);
    if (evt)
    {
        Rectangle viewRect;
        getExtent(viewRect);
        std::cout << "EVT ICON ZOOM (" << viewRect.ul.x << "," << viewRect.ul.y << ")x("
                  << viewRect.lr.x << "," << viewRect.lr.y << ")" << std::endl;
    }
}
