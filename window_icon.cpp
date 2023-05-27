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
#include "window_icon.h"
#include "window_icon_palette.h"

WindowIconClose::WindowIconClose(Rectangle &rect) : Button(rect)
{
    clearOptions(VIEW_OPT_VALIDATE);
}

void WindowIconClose::draw()
{
    Rectangle viewRect;
    ViewRender *renderer = GRenderer;
    Palette *palette = GPaletteGroup->getPalette(PaletteGroup::PAL_WINICON);
    getExtent(viewRect);
    globalize(viewRect);

    unsigned color, color2;
    palette->getPalette(WINICON_BRIGHT, color);
    palette->getPalette(WINICON_DARK, color2);

    // Outer shadow

    /*
     *    BBBBBBBBB
     *    B       D
     *    B       D
     *    B       D
     *    DDDDDDDDD
     */
    Point ul(viewRect.ul);
    renderer->hline(ul, viewRect.width(), color);
    renderer->vline(ul, viewRect.height() - 1, color);
    ul.move(viewRect.width(), 1);
    renderer->vline(ul, viewRect.height() - 1, color2);
    ul.move(-viewRect.width(), viewRect.height() - 1);
    renderer->hline(ul, viewRect.width() - 1, color2);

    // The button frame
    palette->getPalette(WINICON_MAIN, color);
    viewRect.zoom(-1, -1);
    renderer->filledRectangle(viewRect, color);

    if (isDown())
    {
        palette->getPalette(WINICON_PRESSED, color);
    }
    else
    {
        palette->getPalette(WINICON_RELEASED, color);
    }

    viewRect.zoom(-2, -2);
    Point s(viewRect.ul), e(viewRect.lr);
    renderer->line(s, e, color);
    s.move(1, 0);
    e.move(0, -1);
    renderer->line(s, e, color);
    s.move(1, 0);
    e.move(0, -1);
    renderer->line(s, e, color);
    s.move(-2, 1);
    e.move(-1, 2);
    renderer->line(s, e, color);
    s.move(0, 1);
    e.move(-1, 0);
    renderer->line(s, e, color);

    s.x = viewRect.lr.x;
    s.y = viewRect.ul.y;
    e.x = viewRect.ul.x;
    e.y = viewRect.lr.y;
    renderer->line(s, e, color);
    s.move(-1, 0);
    e.move(0, -1);
    renderer->line(s, e, color);
    s.move(-1, 0);
    e.move(0, -1);
    renderer->line(s, e, color);
    s.move(2, 1);
    e.move(1, 2);
    renderer->line(s, e, color);
    s.move(0, 1);
    e.move(1, 0);
    renderer->line(s, e, color);
}

void WindowIconClose::handleEvent(Event *evt)
{
    View::handleEvent(evt);

    if (isEventPositional(evt))
    {
        if (isEventPositionInRange(evt))
        {
            // Update the pressure state, if the new state is RELEASED, it means
            // the icon was pressed and then released, in this case the object
            // will generate an event.
            bool pressed = evt->testPositionalEventStatus(POS_EVT_PRESSED | POS_EVT_LONG);
            if (updateButtonState(pressed))
            {
                setChanged(VIEW_CHANGED_REDRAW);
                /* Now ask for redrawing */
                sendCommand(CMD_REDRAW);
                if (!isDown() && getParent())
                {
                    sendCommand(CMD_CLOSE, BROADCAST_OBJECT, getParent());
                }
            }
            evt->clear();
        }
        else if (isDown())
        {
            updateButtonState(false);
            /* Now ask for drawing */
            sendCommand(CMD_DRAW);
        }
    }
}

WindowIconZoom::WindowIconZoom(Rectangle &rect) : Button(rect)
{
    clearOptions(VIEW_OPT_VALIDATE);
    setResizeMode(VIEW_RESIZE_UX | VIEW_RESIZE_LX);
}

void WindowIconZoom::draw()
{
    Rectangle viewRect;
    ViewRender *renderer = GRenderer;
    Palette *palette = GPaletteGroup->getPalette(PaletteGroup::PAL_WINICON);
    getExtent(viewRect);
    globalize(viewRect);

    unsigned color, color2;
    palette->getPalette(WINICON_BRIGHT, color);
    palette->getPalette(WINICON_DARK, color2);

    // Outer shadow

    /*
     *    BBBBBBBBB
     *    B       D
     *    B       D
     *    B       D
     *    DDDDDDDDD
     */
    Point ul(viewRect.ul);
    renderer->hline(ul, viewRect.width(), color);
    renderer->vline(ul, viewRect.height() - 1, color);
    ul.move(viewRect.width(), 1);
    renderer->vline(ul, viewRect.height() - 1, color2);
    ul.move(-viewRect.width(), viewRect.height() - 1);
    renderer->hline(ul, viewRect.width() - 1, color2);

    // The button frame
    palette->getPalette(WINICON_MAIN, color);
    viewRect.zoom(-1, -1);
    renderer->filledRectangle(viewRect, color);

    if (isDown())
    {
        palette->getPalette(WINICON_PRESSED, color);
    }
    else
    {
        palette->getPalette(WINICON_RELEASED, color);
    }

    if (getParent() && !getParent()->getResizeMode(VIEW_ZOOMED))
    {
        viewRect.zoom(-3, -3);
        renderer->rectangle(viewRect, color);
        viewRect.zoom(-1, -1);
        renderer->rectangle(viewRect, color);
        viewRect.zoom(-1, -1);
        renderer->hline(viewRect.ul, viewRect.width(), color);
    }
    else
    {
        viewRect.zoom(-6, -6);
        viewRect.move(-1, 1);
        renderer->rectangle(viewRect, color);
        viewRect.zoom(-1, -1);
        renderer->hline(viewRect.ul, viewRect.width(), color);
    }
}

void WindowIconZoom::handleEvent(Event *evt)
{
    View::handleEvent(evt);

    if (isEventPositional(evt))
    {
        if (isEventPositionInRange(evt))
        {
            // Update the pressure state, if the new state is RELEASED, it means
            // the icon was pressed and then released, in this case the object
            // will generate an event and toggle isZoom.
            bool pressed = evt->testPositionalEventStatus(POS_EVT_PRESSED | POS_EVT_LONG);
            if (updateButtonState(pressed))
            {
                setChanged(VIEW_CHANGED_REDRAW);
                /* Now ask for redrawing */
                sendCommand(CMD_REDRAW);
                if (!isDown())
                {
                    if (getParent())
                    {
                        sendCommand(getParent()->getResizeMode(VIEW_ZOOMED) ? CMD_RESTORE : CMD_MAXIMIZE, getParent(), getParent());
                    }
                }
            }
            evt->clear();
        }
        else if (isDown())
        {
            updateButtonState(false);
            /* Now ask for drawing */
            sendCommand(CMD_DRAW);
        }
    }
}
