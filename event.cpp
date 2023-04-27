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
#include <iostream>

#include "event.h"

Event::Event(const PositionalEvent &pos)
{
    myEventData.what = EVT_POS;
    myEventData.position = pos;
}

Event::Event(const KeybEvent &kbd)
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

bool Event::isEventKey()
{
    return (myEventData.what == EVT_KBD) ? true : false;
}

bool Event::isEventCommand()
{
    return (myEventData.what == EVT_CMD) ? true : false;
}

bool Event::isEventUnknown()
{
    return (myEventData.what == EVT_UNKNOWN) ? true : false;
}

struct PositionalEvent *Event::getPositionalEvent()
{
    return (myEventData.what == EVT_POS) ? &myEventData.position : nullptr;
}

struct KeybEvent *Event::getKeyDownEvent()
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

void Event::setKeyDownEvent(const KeybEvent &kbd)
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

void Event::print()
{
    switch (myEventData.what)
    {
    case EVT_POS:
    {
        std::cout << "POS: " << myEventData.position.x << "," << myEventData.position.y;
        std::string temp("");
        if (myEventData.position.status & POS_EVT_PRESSED)
            temp += "pressed ";
        if (myEventData.position.status & POS_EVT_DOUBLE)
            temp += "double ";
        if (myEventData.position.status & POS_EVT_LONG)
            temp += "long ";
        if (myEventData.position.status == 0)
            temp += "none ";
        std::cout << " STATUS " << temp;
        temp.clear();
        if (myEventData.position.buttons & POS_EVT_RIGHT)
            temp += "right";
        if (myEventData.position.buttons & POS_EVT_MIDDLE)
            temp += "middle";
        if (myEventData.position.buttons & POS_EVT_LEFT)
            temp += "left";
        if (myEventData.position.buttons & POS_EVT_TAP)
            temp += "tap";
        std::cout << " BUTTONS " << temp << std::endl;
    }
    break;
    case EVT_KBD:
        std::cout << "KBD: KEYCODE " << std::hex << myEventData.keyDown.keyCode;
        std::cout << " MOD " << myEventData.keyDown.modifier << std::dec << std::endl;
        break;
    case EVT_CMD:
        std::cout << "CMD: " << std::hex << myEventData.message.command << "," << myEventData.message.subCommand;
        std::cout << " SENDER: " << myEventData.message.senderObject;
        std::cout << " DEST: " << myEventData.message.destObject;
        std::cout << " TARGET: " << myEventData.message.targetObject;
        std::cout << std::dec << std::endl;
        break;
    case EVT_UNKNOWN:
    default:
        std::cout << "UNKNOWN" << std::endl;
        break;
    }
}