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

#include "scrollbar.h"
#include "scrollbar_palette.h"

ScrollBar::ScrollBar(Rectangle &viewLimits) : View(viewLimits), refElements(1), refSize(1), refPosition(0), activePad(viewLimits), lastPressure(0, 0)
{
}

void ScrollBar::draw()
{
	Rectangle viewRect;
	getExtent(viewRect);
	globalize(viewRect);
	unsigned color, color2;
	ViewRender *renderer = getRenderer();
	Palette *palette = getPalette();

	Rectangle temp(viewRect);

	palette->getPalette(SCROLLBAR_BRIGHT, color);
	palette->getPalette(SCROLLBAR_DARK, color2);

	// Outer shadow

	/*
	 *    BBBBBBBBB
	 *    B       D
	 *    B       D
	 *    B       D
	 *    DDDDDDDDD
	 */
	Point ul(temp.ul);
	renderer->hline(ul, temp.width(), color);
	renderer->vline(ul, temp.height() - 1, color);
	ul.move(temp.width(), 1);
	renderer->vline(ul, temp.height() - 1, color2);
	ul.move(-temp.width(), temp.height() - 1);
	renderer->hline(ul, temp.width() - 1, color2);

	temp.zoom(-1, -1);
	ul = temp.ul;
	renderer->hline(ul, temp.width(), color);
	renderer->vline(ul, temp.height() - 1, color);
	ul.move(temp.width(), 1);
	renderer->vline(ul, temp.height() - 1, color2);
	ul.move(-temp.width(), temp.height() - 1);
	renderer->hline(ul, temp.width() - 1, color2);

	// The pad
	palette->getPalette(SCROLLBAR_FG, color);
	temp.zoom(-1, -1);
	renderer->filledRectangle(temp, color);
}

void ScrollBar::handleEvent(Event *evt)
{
	View::handleEvent(evt);

	if (isEventPositional(evt))
	{
		if (isEventPositionInRange(evt))
		{
			uint8_t status = evt->getPositionalEvent()->status;

			if (status & POS_EVT_PRESSED)
			{
				Point newPressure(evt->getPositionalEvent()->x, evt->getPositionalEvent()->y);
				Point deltaPressure(newPressure);
				if (lastPressure != newPressure)
				{
					updateActivePad(newPressure);
					computeAttributes();
					lastPressure = newPressure;
					/* Now ask for redrawing of the parent */
					sendCommand(CMD_DRAW, getParent(), getParent());
				}
			}
			evt->clear();
		}
	}
}

void ScrollBar::setRefElements(unsigned newval)
{
	if (newval != refElements)
	{
		refElements = newval;
		reDraw();
	}
}

void ScrollBar::setRefSize(unsigned newval)
{
	if (newval != refSize)
	{
		refSize = newval;
		reDraw();
	}
}

void ScrollBar::setRefPosition(unsigned newval)
{
	if (newval != refPosition)
	{
		refPosition = newval;
		reDraw();
	}
}

unsigned ScrollBar::getRefElements() const
{
	return refElements;
}

unsigned ScrollBar::getRefSize() const
{
	return refSize;
}

unsigned ScrollBar::getRefPosition() const
{
	return refPosition;
}

VScrollBar::VScrollBar(Rectangle &viewLimits) : ScrollBar(viewLimits)
{
}

void VScrollBar::updateActivePad(Point &newpos)
{
	Rectangle viewRect;
	getExtent(viewRect);
	int y = newpos.y - lastPressure.y;

	activePad.moveClipped(0, y, viewRect);
}

void VScrollBar::computeActivePad()
{
	int pos;
	Rectangle viewRect;
	getExtent(viewRect);
	activePad = viewRect;

	activePad.zoom(-1, refSize * viewRect.height() / refElements);
	if (refPosition < refSize)
	{
		pos = 0;
	}
	else if (refPosition > refElements - refSize)
	{
		pos = viewRect.height() - activePad.height();
	}
	else
	{
		pos = refPosition * viewRect.height() / refElements;
	}
	activePad.move(0, pos);
}

void VScrollBar::computeAttributes()
{
	Rectangle viewRect;
	getExtent(viewRect);
}