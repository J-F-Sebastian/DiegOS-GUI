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
#include <cstring>

#include "viewinstances.h"
#include "titlebar.h"
#include "titlebar_palette.h"

TitleBar::TitleBar(Rectangle &rect, const char *title) : View(rect), title(title), lastPressure(0, 0)
{
	setOptions(VIEW_OPT_SELECTABLE);
	setResizeMode(VIEW_RESIZE_LX);
}

void TitleBar::drawView()
{
	unsigned color[2];
	Rectangle viewRect;
	ViewRender *r = GRenderer;
	Palette *p = GPaletteGroup->getPalette(PaletteGroup::PAL_TITLEBAR);
	getViewport(viewRect);

	p->getPalette(TITELBAR_BRIGHT, color[0]);
	p->getPalette(TITLEBAR_DARK, color[1]);

	// Outer shadow

	/*
	 *    BBBBBBBBB
	 *    B       D
	 *    B       D
	 *    B       D
	 *    DDDDDDDDD
	 */
	r->frame(viewRect, color, false);
	viewRect.zoom(-1, -1);
	r->frame(viewRect, color, false);

	if (getState(VIEW_STATE_FOREGROUND))
	{
		p->getPalette(TITLEBAR_BG, color[0]);
		p->getPalette(TITLEBAR_TEXT, color[1]);
	}
	else
	{
		p->getPalette(TITLEBAR_BG_DISABLED, color[0]);
		p->getPalette(TITLEBAR_TEXT_DISABLED, color[1]);
	}

	Rectangle vr;
	getViewport(vr);
	vr.zoom(-2, -2);
	// The titlebar frame
	r->filledRectangle(vr, color[0]);
	// The title
	Rectangle temp;
	r->textBox(title.c_str(), temp);
	temp.move(vr.ul.x, vr.ul.y);
	temp.center(vr);
	r->text(temp, color[1], color[0], title.c_str());
}

static inline bool checkAll(uint8_t status, uint8_t test)
{
	return ((status & test) == test) ? true : false;
}

void TitleBar::handleEvent(Event *evt)
{
	View::handleEvent(evt);

	if (getParent() && isEventPositional(evt))
	{
		uint8_t status = evt->getPositionalEvent()->status;
		if (checkAll(status, POS_EVT_DOUBLE | POS_EVT_RELEASED))
		{
			sendCommand((getParent()->getResizeMode(VIEW_ZOOMED)) ? CMD_RESTORE : CMD_MAXIMIZE, getParent(), getParent());
		}
		else if (checkAll(status, POS_EVT_PRESSED | POS_EVT_DRAG))
		{
			Point newPressure(evt->getPositionalEvent()->x, evt->getPositionalEvent()->y);
			if (!getParent()->getState(VIEW_STATE_DRAGGING))
			{
				lastPressure = newPressure;
				getParent()->setState(VIEW_STATE_DRAGGING);
			}
			else if (lastPressure != newPressure)
			{
				Point deltaPressure(newPressure);
				deltaPressure -= lastPressure;
				lastPressure = newPressure;

				getParent()->moveLocation(deltaPressure);
				/* Now ask for redrawing */
				sendCommand(CMD_REDRAW);
			}
		}
		else if (getParent()->getState(VIEW_STATE_DRAGGING))
		{
			/* Reset dragging */
			getParent()->clearState(VIEW_STATE_DRAGGING);
		}
	}
}

void TitleBar::setTitle(const char *ntitle)
{
	size_t len = strlen(ntitle) + 1;
	if (len > 64)
		len = 64;
	this->title.assign(ntitle, len);
}