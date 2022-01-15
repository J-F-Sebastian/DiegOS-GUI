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

#ifndef _WINDOW_H_
#define _WINDOW_H_

#include "view.h"

/*
 * Palette for windows, bright dark and main are used to draw the enveloping frame,
 * Title colors are used for the window title bar, icons is the background of button icons
 * controlling zoom close etc.
 * Window text is the default text color for fonts printed inside the window space.
 */
enum
{
    WINDOW_BRIGHT,
    WINDOW_DARK,
    WINDOW_MAIN,
    WINDOW_TITLE_BG,
    WINDOW_TITLE_TXT,
    WINDOW_ICONS_BG,
    WINDOW_TEXT,
    WINDOW_PAL_NUM
};

/*
 * Window control flags.
 * They dictates which abilites the window have:
 *     the window can be moved around by dragging the title bar
 *     the window can be resized by dragging the lower right corner
 *     the window can be closed
 *     the window can be zoomed in and out
 */
enum
{
    WINDOW_MOVE = (1 << 0),
    WINDOW_GROW = (1 << 1),
    WINDOW_CLOSE = (1 << 2),
    WINDOW_ZOOM = (1 << 3),
    WINDOW_DEFAULT = (WINDOW_MOVE | WINDOW_GROW | WINDOW_CLOSE | WINDOW_ZOOM)
};

class Window : public ViewGroup
{
public:
    Window(Rectangle &viewLimits, const char *title, ViewRender *rnd, PaletteGroup *pals, View *parent, unsigned char ctrlflags = WINDOW_DEFAULT);

protected:
    bool canMove() { return (wFlags & WINDOW_MOVE) ? true : false; }
    bool canGrow() { return (wFlags & WINDOW_GROW) ? true : false; }
    bool canClose() { return (wFlags & WINDOW_CLOSE) ? true : false; }
    bool canZoom() { return (wFlags & WINDOW_ZOOM) ? true : false; }

private:
    unsigned char wFlags;
    bool isZoomed;
};

#endif