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
#include <cstring>

#include "titlebar.h"
#include "titlebar_palette.h"

TitleBar::TitleBar(Rectangle &rect, const char *title) : View(rect), title(title), lastPressure(0, 0)
{
    setOptions(VIEW_OPT_SELECTABLE);
    setResizeMode(VIEW_RESIZE_LX);
}

void TitleBar::draw()
{
    unsigned color, color2;
    Rectangle viewRect;
    ViewRender *r = getRenderer();
    Palette *p = getPalette();
    getExtent(viewRect);
    globalize(viewRect);

    if (getState(VIEW_STATE_FOREGROUND))
    {
        p->getPalette(TITLEBAR_BG, color);
        p->getPalette(TITLEBAR_TEXT, color2);
    }
    else
    {
        p->getPalette(TITLEBAR_BG_DISABLED, color);
        p->getPalette(TITLEBAR_TEXT_DISABLED, color2);
    }
    // The titlebar frame
    r->filledRectangle(viewRect, color);
    // The title
    Rectangle temp;
    r->textBox(title.c_str(), temp);
    temp.move(viewRect.ul.x, viewRect.ul.y);
    temp.center(viewRect);
    p->getPalette(TITLEBAR_TEXT, color2);
    r->text(temp, color, title.c_str());
}

void TitleBar::handleEvent(Event *evt)
{
    View::handleEvent(evt);

    if (isEventPositional(evt))
    {
        if (isEventPositionInRange(evt))
        {
            uint8_t status = evt->getPositionalEvent()->status;
            if (status & POS_EVT_DOUBLE)
            {
                if (getParent())
                {
                    sendCommand((getParent()->getResizeMode(VIEW_ZOOMED)) ? CMD_RESTORE : CMD_MAXIMIZE, getParent(), getParent());
                }
            }
            else if (status & POS_EVT_LONG)
            {
                Point newPressure(evt->getPositionalEvent()->x, evt->getPositionalEvent()->y);
                Point deltaPressure(newPressure);
                deltaPressure -= lastPressure;
                if (!getState(VIEW_STATE_DRAGGING))
                {
                    lastPressure = newPressure;
                    setState(VIEW_STATE_DRAGGING);
                }
                else if (lastPressure != newPressure)
                {
                    lastPressure = newPressure;
                    std::cout << deltaPressure.x << " " << deltaPressure.y << std::endl;
                    if (getParent())
                        getParent()->moveLocation(deltaPressure);
                    /* Now ask for redrawing */
                    sendCommand(CMD_DRAW);
                }
            }
            else
            {
                /* Reset dragging */
                clearState(VIEW_STATE_DRAGGING);
            }
            evt->clear();
        }
        else
        {
            /* Reset dragging */
            clearState(VIEW_STATE_DRAGGING);
        }
    }
}

void TitleBar::setTitle(const char *title)
{
    size_t len = strlen(title) + 1;
    if (len > 64)
        len = 64;
    this->title.assign(title, len);
}