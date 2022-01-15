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

#ifndef _WINDOW_ICON_H_
#define _WINDOW_ICON_H_

#include "view.h"

enum
{
    WINICON_BRIGHT,
    WINICON_DARK,
    WINICON_MAIN,
    WINICON_PRESSED,
    WINICON_RELEASED,
    WINICON_PAL_NUM
};

class WindowIcon : public View
{
public:
    enum WindowIconState
    {
        RELEASED,
        PRESSED
    };

    explicit WindowIcon(Rectangle &rect);

    bool isPressed(void) { return (pressState == PRESSED) ? true : false; }

private:
    WindowIconState pressState;
};

class WindowIconClose : public WindowIcon
{
public:
    explicit WindowIconClose(Rectangle &rect) : WindowIcon(rect) {}

    virtual void draw(void) override;

    virtual void handleEvent(Event *evt) override;
};

class WindowIconZoom : public WindowIcon
{
public:
    explicit WindowIconZoom(Rectangle &rect) : WindowIcon(rect), isZoom(true) {}

    virtual void draw(void) override;

    virtual void handleEvent(Event *evt) override;

private:
    bool isZoom;
};

#endif