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

#include "event.h"

Event::Event(const MouseEvent &mouse)
{
    myEventData.what = EVT_MOUSE;
    myEventData.mouse = mouse;
}

Event::Event(const KeyDownEvent &kbd)
{
    myEventData.what = EVT_KBD;
    myEventData.keyDown = kbd;
}

Event::Event(const MessageEvent &cmd)
{
    myEventData.what = EVT_CMD;
    myEventData.message = cmd;
}

enum Event::EventType Event::getEventType()
{
    return myEventData.what;
}

bool Event::isEventPositional()
{
    return (myEventData.what == EVT_MOUSE) ? true : false;
}

struct MouseEvent *Event::getMouseEvent()
{
    if (myEventData.what == EVT_MOUSE)
        return &myEventData.mouse;
    return nullptr;
}

struct KeyDownEvent *Event::getKeyDownEvent()
{
    if (myEventData.what == EVT_KBD)
        return &myEventData.keyDown;
    return nullptr;
}

struct MessageEvent *Event::getMessageEvent()
{
    if (myEventData.what == EVT_CMD)
        return &myEventData.message;
    return nullptr;
}

void Event::setMouseEvent(const MouseEvent &mouse)
{
    myEventData.what = EVT_MOUSE;
    myEventData.mouse = mouse;
}

void Event::setKeyDownEvent(const KeyDownEvent &kbd)
{
    myEventData.what = EVT_KBD;
    myEventData.keyDown = kbd;
}

void Event::setMessageEvent(const MessageEvent &cmd)
{
    myEventData.what = EVT_CMD;
    myEventData.message = cmd;
}