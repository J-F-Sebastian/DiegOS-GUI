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

#include "button.h"

Button::Button(Rectangle &rect) : View(rect), pressState(RELEASED)
{
    setOptions(VIEW_OPT_VALIDATE);
}

void Button::draw()
{
    Rectangle viewRect;
    getExtent(viewRect);
    globalize(viewRect);
    Rectangle temp(viewRect);
    //std::cout << __PRETTY_FUNCTION__ << "(" << viewRect.ul.x << "," << viewRect.ul.y << ")x("
    //          << viewRect.lr.x << "," << viewRect.lr.y << ")" << std::endl;

    unsigned color, color2;
    palette->getPalette(BUTTON_BRIGHT, color);
    palette->getPalette(BUTTON_DARK, color2);

    //Outer shadow
    Point ul(temp.ul);
    renderer->hline(ul, temp.width() - 1, color);
    renderer->vline(ul, temp.height(), color);
    ul.move(temp.width() - 1, 0);
    renderer->vline(ul, temp.height() - 1, color2);
    ul.move(-temp.width() + 2, temp.height() - 1);
    renderer->hline(ul, temp.width() - 2, color2);

    palette->getPalette(BUTTON_MAIN, color);
    temp.zoom(-1, -1);
    //The button frame
    renderer->rectangle(temp, color);
    temp.zoom(-1, -1);
    renderer->rectangle(temp, color);

    temp.zoom(-1, -1);

    if (getOptions(VIEW_STATE_DISABLED))
    {
        palette->getPalette(BUTTON_DISABLED, color);
    }
    else if (getOptions(VIEW_STATE_SELECTED))
    {
        if (isPressed())
        {
            palette->getPalette(BUTTON_PRESSED, color);
        }
        else
        {
            palette->getPalette(BUTTON_SELECTED, color);
        }
    }

    else
        palette->getPalette(BUTTON_MAIN, color);

    renderer->filledRectangle(temp, color);
}

void Button::handleEvent(Event *evt)
{
    View::handleEvent(evt);
    if (evt)
    {
        Rectangle viewRect;
        getExtent(viewRect);
        std::cout << "EVT BUTTON (" << viewRect.ul.x << "," << viewRect.ul.y << ")x("
                  << viewRect.lr.x << "," << viewRect.lr.y << ")" << std::endl;
    }
}
