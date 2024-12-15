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
#include "scrollbar.h"
#include "scrollbar_palette.h"

ScrollBar::ScrollBar(Rectangle &viewLimits, unsigned refElements, unsigned refVisible, unsigned refPosition) : View(viewLimits),
													       refElements(refElements),
													       refVisible(refVisible),
													       refPosition(refPosition),
													       activePad(viewLimits),
													       lastPressure(0, 0)
{
	setOptions(VIEW_OPT_SELECTABLE | VIEW_OPT_TOPSELECT);
}

bool ScrollBar::setLocation(const Rectangle &loc)
{
	if (View::setLocation(loc))
	{
		computeActivePad();
		return true;
	}
	return false;
}

void ScrollBar::handleEvent(Event *evt)
{
	View::handleEvent(evt);

	if (isEventPositional(evt))
	{
		uint8_t status = evt->getPositionalEvent()->status;

		if (status & POS_EVT_PRESSED)
		{
			Point newPressure(evt->getPositionalEvent()->x, evt->getPositionalEvent()->y);
			makeLocal(newPressure);
			if (lastPressure != newPressure)
			{
				updateActivePad(newPressure);
				computeAttributes();
				lastPressure = newPressure;
				/* Now ask for redrawing of the parent */
				setChanged(VIEW_CHANGED_REDRAW);
				sendCommand(CMD_REDRAW);
			}
			evt->clear();
		}
	}
}

void ScrollBar::drawView()
{
	unsigned color[2];
	Rectangle viewRect;
	getViewport(viewRect);
	ViewRender *r = GRenderer;
	Palette *p = GPaletteGroup->getPalette(PaletteGroup::PAL_SCROLLBAR);

	p->getPalette(SCROLLBAR_BRIGHT, color[0]);
	p->getPalette(SCROLLBAR_DARK, color[1]);

	// Outer shadow, 2 pixels

	/*
	 *    BBBBBBBBB
	 *    B       D
	 *    B       D
	 *    B       D
	 *    DDDDDDDDD
	 */

	r->frame(viewRect, 2, color, false);
	viewRect.zoom(-2, -2);
	p->getPalette(SCROLLBAR_BG, color[0]);
	r->filledRectangle(viewRect, color[0]);
	p->getPalette(SCROLLBAR_FG, color[0]);
	r->filledRectangle(activePad, color[0]);
}

void ScrollBar::setRefElements(unsigned newval)
{
	if (newval != refElements)
	{
		refElements = newval;
		computeActivePad();
		/* Now ask for redrawing of the parent */
		setChanged(VIEW_CHANGED_REDRAW);
		sendCommand(CMD_REDRAW);
	}
}

void ScrollBar::setRefVisible(unsigned newval)
{
	if (newval != refVisible)
	{
		refVisible = newval;
		computeActivePad();
		/* Now ask for redrawing of the parent */
		setChanged(VIEW_CHANGED_REDRAW);
		sendCommand(CMD_REDRAW);
	}
}

void ScrollBar::setRefPosition(unsigned newval)
{
	if (newval != refPosition)
	{
		refPosition = newval;
		computeActivePad();
		/* Now ask for redrawing of the parent */
		setChanged(VIEW_CHANGED_REDRAW);
		sendCommand(CMD_REDRAW);
	}
}

unsigned ScrollBar::getRefElements() const
{
	return refElements;
}

unsigned ScrollBar::getRefVisible() const
{
	return refVisible;
}

unsigned ScrollBar::getRefPosition() const
{
	return refPosition;
}

VScrollBar::VScrollBar(Rectangle &viewLimits, unsigned refElements, unsigned refVisible, unsigned refPosition) : ScrollBar(viewLimits, refElements, refVisible, refPosition)
{
	setResizeMode(VIEW_RESIZE_LX | VIEW_RESIZE_LY | VIEW_RESIZE_UX);
	computeActivePad();
}

void VScrollBar::updateActivePad(Point &newpos)
{
	int y = newpos.y - lastPressure.y;

	Rectangle viewRect;
	getExtent(viewRect);
	viewRect.zoom(-2, -2);
	activePad.moveClipped(0, y, viewRect);

	refPosition = y * refElements / viewRect.height();
	if (refPosition > refElements - refVisible)
		refPosition = refElements - refVisible;
}

void VScrollBar::computeActivePad()
{
	Rectangle viewRect;
	getExtent(viewRect);
	viewRect.zoom(-2, -2);
	activePad = viewRect;

	unsigned int height = refVisible * activePad.height() / refElements;
	lastPressure.y = refPosition * activePad.height() / refElements;
	activePad.height(height);
	activePad.moveClipped(0, lastPressure.y, viewRect);
}

void VScrollBar::computeAttributes()
{
	Rectangle viewRect;
	getExtent(viewRect);
}

HScrollBar::HScrollBar(Rectangle &viewLimits, unsigned refElements, unsigned refVisible, unsigned refPosition) : ScrollBar(viewLimits, refElements, refVisible, refPosition)
{
	setResizeMode(VIEW_RESIZE_LX | VIEW_RESIZE_LY | VIEW_RESIZE_UY);
	computeActivePad();
}

void HScrollBar::updateActivePad(Point &newpos)
{
	int x = newpos.x - lastPressure.x;

	Rectangle viewRect;
	getExtent(viewRect);
	viewRect.zoom(-2, -2);
	activePad.moveClipped(x, 0, viewRect);

	refPosition = x * refElements / viewRect.width();
	if (refPosition > refElements - refVisible)
		refPosition = refElements - refVisible;
}

void HScrollBar::computeActivePad()
{
	Rectangle viewRect;
	getExtent(viewRect);
	viewRect.zoom(-2, -2);
	activePad = viewRect;

	unsigned int width = refVisible * activePad.width() / refElements;
	lastPressure.x = refPosition * activePad.width() / refElements;
	activePad.width(width);
	activePad.moveClipped(lastPressure.x, 0, viewRect);
}

void HScrollBar::computeAttributes()
{
	Rectangle viewRect;
	getExtent(viewRect);
}