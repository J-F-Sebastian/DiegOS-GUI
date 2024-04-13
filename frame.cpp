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
#include "frame.h"
#include "frame_palette.h"

Frame::Frame(Rectangle &extent, unsigned width, enum FrameStyle style) : extent(extent), style(style), width(width)
{
	if (style == FRAME_BEVELLED)
	{
		if (width < 5)
			width = 5;
	}
}

void Frame::setExtent(Rectangle &extent)
{
	this->extent = extent;
}

void Frame::setWidth(unsigned pix)
{
	width = pix;
	if (style == FRAME_BEVELLED)
	{
		if (width < 5)
			width = 5;
	}
}

void Frame::draw(void *buffer)
{
	unsigned color, color2;
	ViewRender *r = GRenderer;
	Palette *p = GPaletteGroup->getPalette(PaletteGroup::PAL_FRAME);

	if (style == FRAME_FLAT)
	{
		p->getPalette(FRAME_MAIN, color);
		for (unsigned i = 0; i < width; i++)
		{
			r->rectangle(extent, color);
			extent.zoom(-1, -1);
		}
	}
	else if (style == FRAME_BEVELLED)
	{
		Rectangle temp(extent);

		p->getPalette(FRAME_BRIGHT, color);
		p->getPalette(FRAME_DARK, color2);

		// Outer shadow, 2 pixels

		/*
		 *    BBBBBBBBB
		 *    B       D
		 *    B       D
		 *    B       D
		 *    DDDDDDDDD
		 */
		Point ul(temp.ul);
		r->hline(ul, temp.width(), color);
		r->vline(ul, temp.height() - 1, color);
		ul.move(temp.width(), 1);
		r->vline(ul, temp.height() - 1, color2);
		ul.move(-temp.width(), temp.height() - 1);
		r->hline(ul, temp.width() - 1, color2);

		temp.zoom(-1, -1);
		ul = temp.ul;
		r->hline(ul, temp.width(), color);
		r->vline(ul, temp.height() - 1, color);
		ul.move(temp.width(), 1);
		r->vline(ul, temp.height() - 1, color2);
		ul.move(-temp.width(), temp.height() - 1);
		r->hline(ul, temp.width() - 1, color2);

		// The frame, width - 4 pixels
		p->getPalette(FRAME_MAIN, color);
		for (unsigned i = 0; i < width - 4; i++)
		{
			temp.zoom(-1, -1);
			r->rectangle(temp, color);
		}

		// Inner shadow, 2 pixels

		/*
		 *    DDDDDDDDD
		 *    D       B
		 *    D       B
		 *    D       B
		 *    BBBBBBBBB
		 */
		temp.zoom(-1, -1);
		p->getPalette(FRAME_BRIGHT, color);
		ul = temp.ul;
		r->hline(ul, temp.width(), color2);
		r->vline(ul, temp.height() - 1, color2);
		ul.move(temp.width(), 1);
		r->vline(ul, temp.height() - 1, color);
		ul.move(-temp.width(), temp.height() - 1);
		r->hline(ul, temp.width() - 1, color);

		temp.zoom(-1, -1);
		ul = temp.ul;
		r->hline(ul, temp.width(), color2);
		r->vline(ul, temp.height() - 1, color2);
		ul.move(temp.width(), 1);
		r->vline(ul, temp.height() - 1, color);
		ul.move(-temp.width(), temp.height() - 1);
		r->hline(ul, temp.width() - 1, color);
	}
}