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

Window::Window(Rectangle &viewLimits, const char *title, View *parent, unsigned char ctrlflags) : ViewGroup(viewLimits, parent),
												  wFlags(ctrlflags),
												  isZoomed(false),
												  frame(extent)
{
	setOptions(VIEW_OPT_TOPSELECT | VIEW_OPT_TILEABLE | VIEW_OPT_SELECTABLE);
	setResizeMode(VIEW_RESIZEABLE);
	setGfx(VIEW_GFX_FRAMED);

	View *tmpView = nullptr;
	Rectangle ext;

	getExtent(ext);
	ext.zoom(-4, -4);
	ext.lr.y = 28;

	if (wFlags & WINDOW_CLOSE)
	{
		Rectangle temp(4, 4, 28, 28);
		tmpView = new WindowIconClose(temp);
		insert(tmpView);
		ext.ul.move(25, 0);
	}
	if (wFlags & WINDOW_ZOOM)
	{
		Rectangle temp(4, 4, 28, 28);
		temp.move(ext.width(), 0);
		tmpView = new WindowIconZoom(temp);
		insert(tmpView);
		ext.lr.move(-25, 0);
	}

	tmpView = new TitleBar(ext, title);
	insert(tmpView);

	getExtent(ext);
	makeGlobal(ext.ul);
	makeGlobal(ext.lr);
	frame.setExtent(ext);

	getExtent(ext);
	ext.zoom(-4, -4);
	ext.ul.move(0, 25);
	tmpView = new Background(ext);
	insert(tmpView);
}

void Window::draw()
{
	Rectangle ext = extent;
	makeGlobal(ext.ul);
	makeGlobal(ext.lr);
	frame.setExtent(ext);
	frame.draw(nullptr);
	ViewGroup::draw();
}