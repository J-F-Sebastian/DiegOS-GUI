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

#include "vieweventsdl.h"
#include "SDL.h"

ViewEventSDL::ViewEventSDL() : ViewEventManager()
{
    if (SDL_InitSubSystem(SDL_INIT_EVENTS))
    {
        std::cout << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
    }
    else
    {
        SDL_EventState(SDL_WINDOWEVENT, SDL_IGNORE);
        SDL_EventState(SDL_MOUSEMOTION, SDL_IGNORE);

        myEventType = SDL_RegisterEvents(1);
        if (myEventType == -1U)
        {
            SDL_QuitSubSystem(SDL_INIT_EVENTS);
            std::cout << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
        }
    }
}

ViewEventSDL::~ViewEventSDL()
{
    if (SDL_WasInit(SDL_INIT_EVENTS))
        SDL_QuitSubSystem(SDL_INIT_EVENTS);
}

bool ViewEventSDL::wait(Event *evt, int timeoutms)
{
    int retval;
    SDL_Event sdlevt;

    if (!evt)
        return false;

    SDL_FlushEvents(SDL_APP_TERMINATING, SDL_SYSWMEVENT);
    SDL_FlushEvents(SDL_CLIPBOARDUPDATE, SDL_DROPCOMPLETE);

    if (timeoutms > 0)
        retval = SDL_WaitEventTimeout(&sdlevt, timeoutms);
    else if (timeoutms == 0)
        retval = SDL_PollEvent(&sdlevt);
    else
        retval = SDL_WaitEvent(&sdlevt);

    if (retval)
    {
        switch (sdlevt.type)
        {
        case SDL_KEYDOWN:
        {
            KeyDownEvent kbd;
            kbd.keyCode = sdlevt.key.keysym.scancode;
            evt->setKeyDownEvent(kbd);
        }
        break;

        case SDL_MOUSEBUTTONDOWN:
        /* FALLTHRU */
        case SDL_MOUSEBUTTONUP:
        {
            MouseEvent mouse;
            if (sdlevt.button.button == SDL_BUTTON_LEFT)
                mouse.buttons = 1 << 2;
            if (sdlevt.button.button == SDL_BUTTON_MIDDLE)
                mouse.buttons = 1 << 1;
            if (sdlevt.button.button == SDL_BUTTON_RIGHT)
                mouse.buttons = 1 << 0;
            mouse.pressed = (sdlevt.button.state == SDL_PRESSED) ? true : false;
            mouse.doubleClick = (sdlevt.button.clicks == 2) ? true : false;
            mouse.x = sdlevt.button.x;
            mouse.y = sdlevt.button.y;
            evt->setMouseEvent(mouse);
        }
        break;

        //case SDL_MOUSEWHEEL:
        case SDL_MOUSEMOTION:
        {
            MouseEvent mouse;
            mouse.buttons = 0;
            mouse.doubleClick = false;
            mouse.x = sdlevt.motion.x;
            mouse.y = sdlevt.motion.y;
            evt->setMouseEvent(mouse);
        }
        break;

        default:
            if (sdlevt.type == myEventType)
            {
                MessageEvent cmd;
                memcpy(&cmd, sdlevt.user.data1, sizeof(cmd));
                evt->setMessageEvent(cmd);
                break;
            }
            else
                return false;
        }
        return true;
    }

    std::cout << "SDL event error! SDL_Error: " << SDL_GetError() << std::endl;
    return false;
}

bool ViewEventSDL::poll()
{
    return (SDL_PollEvent(nullptr)) ? true : false;
}

bool ViewEventSDL::put(Event *evt)
{
    SDL_Event event;
    SDL_zero(event);
    event.type = myEventType;
    event.user.code = myEventType;
    event.user.data1 = static_cast<void *>(evt);
    return (SDL_PushEvent(&event)) ? true : false;
}
