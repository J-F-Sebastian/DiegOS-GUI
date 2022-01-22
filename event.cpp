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
#include <cstring>

#include "event.h"

Event::Event(const PositionalEvent &pos)
{
    myEventData.what = EVT_POS;
    myEventData.position = pos;
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
    return (myEventData.what == EVT_POS) ? true : false;
}

struct PositionalEvent *Event::getPositionalEvent()
{
    return (myEventData.what == EVT_POS) ? &myEventData.position : nullptr;
}

struct KeyDownEvent *Event::getKeyDownEvent()
{
    return (myEventData.what == EVT_KBD) ? &myEventData.keyDown : nullptr;
}

struct MessageEvent *Event::getMessageEvent()
{
    return (myEventData.what == EVT_CMD) ? &myEventData.message : nullptr;
}

void Event::setPositionalEvent(const PositionalEvent &pos)
{
    myEventData.what = EVT_POS;
    myEventData.position = pos;
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

bool Event::testPositionalEventPos(char bitmap)
{
    if (myEventData.what == EVT_POS)
    {
        if (myEventData.position.buttons & bitmap)
            return true;
    }
    return false;
}
bool Event::testPositionalEventStatus(char bitmap)
{
    if (myEventData.what == EVT_POS)
    {
        if (myEventData.position.status & bitmap)
            return true;
    }
    return false;
}

void Event::clear()
{
    memset(&myEventData, 0, sizeof(myEventData));
    myEventData.what = EVT_UNKNOWN;
}