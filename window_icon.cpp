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

WindowIconClose::WindowIconClose(Rectangle &rect) : Button(rect)
{
    clearOptions(VIEW_OPT_TOPSELECT | VIEW_OPT_VALIDATE);
}

void WindowIconClose::draw()
{
    Rectangle viewRect;
    ViewRender *renderer = getRenderer();
    Palette *palette = getPalette();
    getExtent(viewRect);
    globalize(viewRect);

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
    Point ul(viewRect.ul);
    renderer->hline(ul, viewRect.width(), color);
    renderer->vline(ul, viewRect.height() - 1, color);
    ul.move(viewRect.width(), 1);
    renderer->vline(ul, viewRect.height() - 1, color2);
    ul.move(-viewRect.width(), viewRect.height() - 1);
    renderer->hline(ul, viewRect.width() - 1, color2);

    //The button frame
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

    if (isEventPositionValid(evt))
    {
        // Update the pressure state, if the new state is RELEASED, it means
        // the icon was pressed and then released, in this case the object
        // will generate an event.
        bool pressed = evt->testPositionalEventStatus(POS_EVT_PRESSED);
        if (updateButtonState(pressed) && !isDown() && getParent())
        {
            Event evt2;
            MessageEvent cmd;

            cmd.senderObject = this;
            cmd.destObject = getParent();
            cmd.targetObject = getParent();
            cmd.command = CMD_CLOSE;
            evt2.setMessageEvent(cmd);
            sendEvent(&evt2);
        }
        evt->clear();
    }
    else if (isDown())
    {
        updateButtonState(false);
    }
}

WindowIconZoom::WindowIconZoom(Rectangle &rect) : Button(rect), isZoom(true)
{
    clearOptions(VIEW_OPT_TOPSELECT | VIEW_OPT_VALIDATE);
    setResizeMode(VIEW_RESIZE_UX | VIEW_RESIZE_LX);
}

void WindowIconZoom::draw()
{
    Rectangle viewRect;
    ViewRender *renderer = getRenderer();
    Palette *palette = getPalette();
    getExtent(viewRect);
    globalize(viewRect);

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
    Point ul(viewRect.ul);
    renderer->hline(ul, viewRect.width(), color);
    renderer->vline(ul, viewRect.height() - 1, color);
    ul.move(viewRect.width(), 1);
    renderer->vline(ul, viewRect.height() - 1, color2);
    ul.move(-viewRect.width(), viewRect.height() - 1);
    renderer->hline(ul, viewRect.width() - 1, color2);

    //The button frame
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

    if (isZoom)
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

    if (isEventPositionValid(evt))
    {
        // Update the pressure state, if the new state is RELEASED, it means
        // the icon was pressed and then released, in this case the object
        // will generate an event and toggle isZoom.
        bool pressed = evt->testPositionalEventStatus(POS_EVT_PRESSED);
        if (updateButtonState(pressed) && !isDown())
        {
            if (getParent())
            {
                Event evt2;
                MessageEvent cmd;

                cmd.senderObject = this;
                cmd.destObject = getParent();
                cmd.targetObject = getParent();
                cmd.command = (isZoom) ? CMD_MAXIMIZE : CMD_RESTORE;
                evt2.setMessageEvent(cmd);
                sendEvent(&evt2);
            }
            isZoom = !isZoom;
            draw();
        }
        evt->clear();
    }
    else if (isDown())
    {
        updateButtonState(false);
        draw();
    }
}
