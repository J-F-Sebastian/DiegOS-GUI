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

WindowIconClose::WindowIconClose(Rectangle &rect) : AbstractButton(rect)
{
	clearOptions(VIEW_OPT_VALIDATE);
}

void WindowIconClose::drawView()
{
	Rectangle viewRect;
	ViewRender *renderer = GRenderer;
	Palette *palette = GPaletteGroup->getPalette(PaletteGroup::PAL_WINICON);
	getViewport(viewRect);

	unsigned color[2];
	palette->getPalette(WINICON_BRIGHT, color[0]);
	palette->getPalette(WINICON_DARK, color[1]);

	// Outer shadow

	/*
	 *    BBBBBBBBB
	 *    B       D
	 *    B       D
	 *    B       D
	 *    DDDDDDDDD
	 */
	renderer->frame(viewRect, 2, color, false);

	// The button frame
	palette->getPalette(WINICON_MAIN, color[0]);
	viewRect.zoom(-2, -2);
	renderer->filledRectangle(viewRect, color[0]);

	if (isDown())
	{
		palette->getPalette(WINICON_PRESSED, color[0]);
	}
	else
	{
		palette->getPalette(WINICON_RELEASED, color[0]);
	}

	viewRect.zoom(-2, -2);
	Point s(viewRect.ul), e(viewRect.lr);
	renderer->line(s, e, color[0]);
	s.move(1, 0);
	e.move(0, -1);
	renderer->line(s, e, color[0]);
	s.move(1, 0);
	e.move(0, -1);
	renderer->line(s, e, color[0]);
	s.move(-2, 1);
	e.move(-1, 2);
	renderer->line(s, e, color[0]);
	s.move(0, 1);
	e.move(-1, 0);
	renderer->line(s, e, color[0]);

	s.x = viewRect.lr.x;
	s.y = viewRect.ul.y;
	e.x = viewRect.ul.x;
	e.y = viewRect.lr.y;
	renderer->line(s, e, color[0]);
	s.move(-1, 0);
	e.move(0, -1);
	renderer->line(s, e, color[0]);
	s.move(-1, 0);
	e.move(0, -1);
	renderer->line(s, e, color[0]);
	s.move(2, 1);
	e.move(1, 2);
	renderer->line(s, e, color[0]);
	s.move(0, 1);
	e.move(1, 0);
	renderer->line(s, e, color[0]);
}

void WindowIconClose::doAction()
{
	if (getParent())
	{
		sendCommand(CMD_CLOSE, getParent(), getParent());
	}
}

WindowIconZoom::WindowIconZoom(Rectangle &rect) : AbstractButton(rect)
{
	clearOptions(VIEW_OPT_VALIDATE);
	setResizeMode(VIEW_RESIZE_UX | VIEW_RESIZE_LX);
}

void WindowIconZoom::drawView()
{
	Rectangle viewRect;
	ViewRender *renderer = GRenderer;
	Palette *palette = GPaletteGroup->getPalette(PaletteGroup::PAL_WINICON);
	getViewport(viewRect);

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
		renderer->rectangle(viewRect, 2, color);
		viewRect.zoom(-2, -2);
		renderer->hline(viewRect.ul, viewRect.width(), color);
	}
	else
	{
		viewRect.zoom(-6, -6);
		viewRect.move(-1, 1);
		renderer->rectangle(viewRect, 1, color);
		viewRect.zoom(-1, -1);
		renderer->hline(viewRect.ul, viewRect.width(), color);
	}
}

void WindowIconZoom::doAction()
{

	if (getParent())
	{
		sendCommand(/*getParent()->getResizeMode(VIEW_ZOOMED) ? CMD_RESTORE : CMD_MAXIMIZE*/ CMD_ZOOM, getParent(), getParent());
	}
}