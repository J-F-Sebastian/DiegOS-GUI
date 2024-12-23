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
#include "resizetab.h"

Window::Window(Rectangle &viewLimits, const char *title, View *parent, unsigned char ctrlflags) : ViewGroup(viewLimits, VIEW_IS_FRAMED | VIEW_IS_SOLID | VIEW_IS_SHADOWED, parent),
												  wFlags(ctrlflags),
												  isZoomed(false)
{
	setOptions(VIEW_OPT_TOPSELECT | VIEW_OPT_TILEABLE | VIEW_OPT_SELECTABLE);
	setResizeMode(VIEW_RESIZEABLE);

	View *tmpView = nullptr;
	Rectangle ext;

	getViewport(ext);

	Rectangle ic = ext;
	Rectangle iz = ext;
	Rectangle rt = ext;

	ic.resize(24, 24);
	iz.resize(24, 24);
	rt.resize(24, 24);
	iz.move(ext.width() - 24, 0);
	rt.move(ext.width() - 24, ext.height() - 24);

	if (wFlags & WINDOW_CLOSE)
	{
		tmpView = new WindowIconClose(ic);
		insert(tmpView);
		ext.ul.move(ic.width(), 0);
	}
	if (wFlags & WINDOW_ZOOM)
	{
		tmpView = new WindowIconZoom(iz);
		insert(tmpView);
		ext.lr.move(-iz.width(), 0);
	}

	View *tmpView1 = new ResizeTab(rt);
	insert(tmpView1);

	ext.height(24);
	tmpView = new TitleBar(ext, title);
	insert(tmpView);

	getViewport(ext);
	ext.ul.move(ext.width() - 24, 25);
	ext.lr.move(0, -24);
	View *tmpView2 = new VScrollBar(ext, 16, 4, 1);
	insert(tmpView2);

	getViewport(ext);
	ext.ul.move(0, ext.height() - 24);
	ext.lr.move(-24, 0);
	View *tmpView3 = new HScrollBar(ext, 16, 4, 1);
	insert(tmpView3);
}