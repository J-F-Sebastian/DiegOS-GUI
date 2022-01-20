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
    setOptions(VIEW_OPT_SELECTABLE | VIEW_OPT_TOPSELECT | VIEW_OPT_VALIDATE);
}

void Button::draw()
{
    Rectangle viewRect;
    getExtent(viewRect);
    globalize(viewRect);
    Rectangle temp(viewRect);

    unsigned color, color2;
    palette->getPalette(BUTTON_BRIGHT, color);
    palette->getPalette(BUTTON_DARK, color2);

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

    //Inner shadow

    /*
     *    DDDDDDDDD
     *    D       B
     *    D       B
     *    D       B
     *    BBBBBBBBB
     */
    temp.zoom(-1, -1);
    ul = temp.ul;
    renderer->hline(ul, temp.width(), color2);
    renderer->vline(ul, temp.height() - 1, color2);
    ul.move(temp.width(), 1);
    renderer->vline(ul, temp.height() - 1, color);
    ul.move(-temp.width(), temp.height() - 1);
    renderer->hline(ul, temp.width() - 1, color);

    //Outer shadow

    /*
     *    BBBBBBBBB
     *    B       D
     *    B       D
     *    B       D
     *    DDDDDDDDD
     */
    temp.zoom(-1, -1);
    ul = temp.ul;
    renderer->hline(ul, temp.width(), color);
    renderer->vline(ul, temp.height() - 1, color);
    ul.move(temp.width(), 1);
    renderer->vline(ul, temp.height() - 1, color2);
    ul.move(-temp.width(), temp.height() - 1);
    renderer->hline(ul, temp.width() - 1, color2);

    temp.zoom(-1, -1);
    if (getState(VIEW_STATE_DISABLED))
    {
        palette->getPalette(BUTTON_DISABLED, color);
        renderer->filledRectangle(temp, color);
    }
    else if (getState(VIEW_STATE_SELECTED))
    {
        if (isPressed())
        {
            palette->getPalette(BUTTON_PRESSED, color);
        }
        else
        {
            palette->getPalette(BUTTON_SELECTED, color);
        }
        renderer->filledRectangle(temp, color);
    }
    else
    {
        palette->getPalette(BUTTON_MAIN, color);
        renderer->filledRectangle(temp, color);
    }
}

void Button::handleEvent(Event *evt)
{
    View::handleEvent(evt);
    if (evt)
    {
        if (isEventPositionValid(evt))
        {
            MouseEvent *mouse = evt->getMouseEvent();
            if ((pressState == RELEASED) && (mouse->pressed))
            {
                pressState = PRESSED;
                draw();
            }
            else if ((pressState == PRESSED) && (!mouse->pressed))
            {
                pressState = RELEASED;
                draw();
            }
        }
    }
}
