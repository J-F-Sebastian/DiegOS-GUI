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

TitleBar::TitleBar(Rectangle &rect, const char *title) : View(rect), title(title), isZoom(false)
{
    setResizeMode(VIEW_RESIZE_LX);
}

void TitleBar::draw()
{
    unsigned color;
    Rectangle viewRect;
    getExtent(viewRect);
    globalize(viewRect);

    palette->getPalette(TITLEBAR_BG, color);

    //The titlebar frame
    renderer->filledRectangle(viewRect, color);
    //The title
    Rectangle temp;
    renderer->textBox(title.c_str(), temp);
    temp.move(viewRect.ul.x, viewRect.ul.y);
    temp.center(viewRect);
    palette->getPalette(TITLEBAR_TEXT, color);
    renderer->text(temp, color, title.c_str());
}

void TitleBar::handleEvent(Event *evt)
{
    View::handleEvent(evt);
    if (isEventPositionValid(evt))
    {
        if ((evt->getPositionalEvent()->status & (POS_EVT_DOUBLE | POS_EVT_PRESSED)) == POS_EVT_DOUBLE)
        {
            if (parentView)
            {
                Event evt2;
                MessageEvent cmd;

                cmd.senderObject = this;
                cmd.destObject = parentView;
                cmd.targetObject = parentView;
                cmd.command = (isZoom) ? CMD_MAXIMIZE : CMD_RESTORE;
                evt2.setMessageEvent(cmd);
                sendEvent(&evt2);
            }
            isZoom = !isZoom;
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