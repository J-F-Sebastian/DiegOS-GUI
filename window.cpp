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

#include "window.h"
#include "window_palette.h"
#include "window_icon.h"
#include "frame.h"
#include "titlebar.h"
#include "background.h"
#include "scrollbar.h"

Window::Window(Rectangle &viewLimits, const char *title, View *parent, unsigned char ctrlflags) : ViewGroup(viewLimits, VIEW_IS_BUFFERED, parent), wFlags(ctrlflags), isZoomed(false)
{
	setOptions(VIEW_OPT_TOPSELECT | VIEW_OPT_TILEABLE | VIEW_OPT_SELECTABLE);
	setResizeMode(VIEW_RESIZEABLE);

	View *tmpView = nullptr;
	Rectangle ext;

	getExtent(ext);
	tmpView = new Frame(ext, 6);
	insert(tmpView);

	ext.zoom(-6, -6);
	ext.ul.move(0, 25);
	tmpView = new Background(ext);
	insert(tmpView);

	getExtent(ext);
	ext.zoom(-6, -6);
	ext.lr.y = 30;

	if (wFlags & WINDOW_CLOSE)
	{
		Rectangle temp(6, 6, 30, 30);
		tmpView = new WindowIconClose(temp);
		insert(tmpView);
		ext.ul.move(temp.width(), 0);
	}
	if (wFlags & WINDOW_ZOOM)
	{
		Rectangle temp(6, 6, 30, 30);
		temp.move(ext.width() - 1, 0);
		tmpView = new WindowIconZoom(temp);
		insert(tmpView);
		ext.lr.move(-temp.width() - 1, 0);
	}

	tmpView = new TitleBar(ext, title);
	insert(tmpView);

	Rectangle temp(175, 75, 200, 190);
	tmpView = new VScrollBar(temp);
	insert(tmpView);
}