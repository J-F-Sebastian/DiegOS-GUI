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

Frame::Frame(Rectangle &rect, unsigned width, enum FrameStyle style) : View(rect), style(style), width(width)
{
	if (style == FRAME_BEVELLED)
	{
		if (width < 5)
			width = 5;
	}
	setResizeMode(VIEW_RESIZEABLE);
	setOptions(VIEW_OPT_SELECTABLE);
	clearAttribute(VIEW_IS_BUFFERED);
}

void Frame::drawView()
{
	Rectangle viewRect;
	getBorders(viewRect);
	unsigned color[2];
	ViewRender *r = GRenderer;
	Palette *p = GPaletteGroup->getPalette(PaletteGroup::PAL_FRAME);

	if (style == FRAME_FLAT)
	{
		p->getPalette(FRAME_MAIN, color[0]);
		for (unsigned i = 0; i < width; i++)
		{
			r->rectangle(viewRect, color[0]);
			viewRect.zoom(-1, -1);
		}
	}
	else if (style == FRAME_BEVELLED)
	{
		Rectangle temp(viewRect);

		p->getPalette(FRAME_BRIGHT, color[0]);
		p->getPalette(FRAME_DARK, color[1]);

		// Outer shadow, 2 pixels

		/*
		 *    BBBBBBBBB
		 *    B       D
		 *    B       D
		 *    B       D
		 *    DDDDDDDDD
		 */

		r->frame(temp, color, false);
		temp.zoom(-1, -1);
		r->frame(temp, color, false);

		// The frame, width - 4 pixels
		p->getPalette(FRAME_MAIN, color[0]);
		for (unsigned i = 0; i < width - 4; i++)
		{
			temp.zoom(-1, -1);
			r->rectangle(temp, color[0]);
		}

		// Inner shadow, 2 pixels

		/*
		 *    DDDDDDDDB
		 *    D       B
		 *    D       B
		 *    D       B
		 *    DBBBBBBBB
		 */
		temp.zoom(-1, -1);
		r->frame(temp, color, true);
		temp.zoom(-1, -1);
		r->frame(temp, color, true);

		if (getParent()->getResizeMode(VIEW_RESIZEABLE))
		{
			temp = viewRect;
			temp.ul = temp.lr;
			temp.ul.move(-6, -6);

			// The active lower right corner
			p->getPalette(FRAME_DRAGGING, color[0]);
			r->filledRectangle(temp, color[0]);
		}
	}
}

void Frame::handleEvent(Event *evt)
{
	View::handleEvent(evt);
}

bool Frame::isEventPositionInRange(Event *evt)
{
	/*
	 * I am somewhat different ...
	 */
	Rectangle lims;
	Point where(evt->getPositionalEvent()->x, evt->getPositionalEvent()->y);

	makeLocal(where);
	getExtent(lims);

	/*
	 * Our limits are different: where is in range if it falls inside the 2 rectangles
	 * defining the borders of the frame.
	 */
	if (lims.includes(where))
	{
		lims.zoom(-(int)width, -(int)width);
		if (!lims.includes(where))
			return true;
	}

	return false;
}

void Frame::computeExposure()
{
	Rectangle ext;
	bool areaSet = true;

	getExtent(ext);

	{
		Rectangle temp(0, 0, ext.width(), width);
		globalize(temp);

		areaSet = areaSet && GZBuffer->isAreaSet(temp);
		GZBuffer->set(temp);

		temp.move(0, ext.height() - width);
		areaSet = areaSet && GZBuffer->isAreaSet(temp);
		GZBuffer->set(temp);
	}

	{
		Rectangle temp(0, width, width, ext.height() - width);
		globalize(temp);

		areaSet = areaSet && GZBuffer->isAreaSet(temp);
		GZBuffer->set(temp);

		temp.move(ext.width() - width, 0);
		areaSet = areaSet && GZBuffer->isAreaSet(temp);
		GZBuffer->set(temp);
	}

	setExposed(!areaSet);
}