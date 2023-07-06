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

#include "viewinstances.h"
#include "button.h"
#include "button_palette.h"

Button::Button(Rectangle &rect) : View(rect), buttonIsDown(false)
{
    setOptions(VIEW_OPT_SELECTABLE | VIEW_OPT_TOPSELECT | VIEW_OPT_VALIDATE);
}

void Button::draw()
{
    Rectangle viewRect;
    ViewRender *r = GRenderer;
    Palette *p = GPaletteGroup->getPalette(PaletteGroup::PAL_BUTTON);
    getExtent(viewRect);
    globalize(viewRect);
    Rectangle temp(viewRect);

    unsigned color, color2;
    p->getPalette(BUTTON_BRIGHT, color);
    p->getPalette(BUTTON_DARK, color2);

    // Outer shadow

    /*
     *    BBBBBBBBB
     *    B       D
     *    B       D
     *    B       D
     *    DDDDDDDDD
     */
    Point ul(temp.ul);
    r->hline(ul, temp.width(), color);
    r->vline(ul, temp.height() - 1, color);
    ul.move(temp.width(), 1);
    r->vline(ul, temp.height() - 1, color2);
    ul.move(-temp.width(), temp.height() - 1);
    r->hline(ul, temp.width() - 1, color2);

    // Inner shadow

    /*
     *    DDDDDDDDD
     *    D       B
     *    D       B
     *    D       B
     *    BBBBBBBBB
     */
    temp.zoom(-1, -1);
    ul = temp.ul;
    r->hline(ul, temp.width(), color2);
    r->vline(ul, temp.height() - 1, color2);
    ul.move(temp.width(), 1);
    r->vline(ul, temp.height() - 1, color);
    ul.move(-temp.width(), temp.height() - 1);
    r->hline(ul, temp.width() - 1, color);

    // Outer shadow

    /*
     *    BBBBBBBBB
     *    B       D
     *    B       D
     *    B       D
     *    DDDDDDDDD
     */
    temp.zoom(-1, -1);
    ul = temp.ul;
    r->hline(ul, temp.width(), color);
    r->vline(ul, temp.height() - 1, color);
    ul.move(temp.width(), 1);
    r->vline(ul, temp.height() - 1, color2);
    ul.move(-temp.width(), temp.height() - 1);
    r->hline(ul, temp.width() - 1, color2);

    temp.zoom(-1, -1);
    if (getState(VIEW_STATE_DISABLED))
    {
        p->getPalette(BUTTON_DISABLED, color);
        r->filledRectangle(temp, color);
    }
    else if (getState(VIEW_STATE_SELECTED))
    {
        if (isDown())
        {
            p->getPalette(BUTTON_PRESSED, color);
        }
        else
        {
            p->getPalette(BUTTON_SELECTED, color);
        }
        r->filledRectangle(temp, color);
    }
    else
    {
        p->getPalette(BUTTON_MAIN, color);
        r->filledRectangle(temp, color);
    }
}

void Button::handleEvent(Event *evt)
{
    View::handleEvent(evt);

    if (isEventPositional(evt))
    {
        // Update the pressure state, if the new state is RELEASED, it means
        // the icon was pressed and then released, in this case the object
        // will generate an event.
        // Skip move-on-hold events.
        if (!evt->testPositionalEventStatus(POS_EVT_DRAG))
        {
            bool pressed = (evt->testPositionalEventStatus(POS_EVT_PRESSED) && evt->testPositionalEventPos(POS_EVT_LEFT));
            std::cout << pressed << std::endl;
            if (updateButtonState(pressed))
            {
                std::cout << "updated" << std::endl;
                if (getParent() && getParent()->getState(VIEW_STATE_FOCUSED))
                {
                    setChanged(VIEW_CHANGED_REDRAW);
                    /* Now ask for redrawing */
                    sendCommand(CMD_REDRAW);
                }
                else
                {
                    /* Now ask for drawing */
                    sendCommand(CMD_DRAW);
                }
            }
            evt->clear();
        }
    }
}

bool Button::updateButtonState(bool eventPressed)
{
    bool toBeUpdated = (buttonIsDown != eventPressed) ? true : false;

    if (toBeUpdated)
    {
        buttonIsDown = !buttonIsDown;
    }

    return toBeUpdated;
}