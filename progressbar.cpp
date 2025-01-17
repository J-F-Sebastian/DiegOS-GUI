/*
 * DiegOS Operating System source code
 *
 * Copyright (C) 2012 - 2024 Diego Gallizioli
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

#include <cstdio>

#include "viewinstances.h"
#include "progressbar.h"
#include "progressbar_palette.h"

ProgressBar::ProgressBar(Rectangle &rect, bool showpercent) : View(rect), showPercent(showpercent), percent(36)
{
}

void ProgressBar::drawView()
{
	unsigned color, color2;
	int pxcent;
	ViewRender *r = GRenderer;
	Palette *p = GPaletteGroup->getPalette(PaletteGroup::PAL_PROGRESSBAR);
	Rectangle viewRect;
	getViewport(viewRect);

	p->getPalette(PROGRESSBAR_BG, color);
	r->filledRectangle(viewRect, color);
	viewRect.zoom(-2, -2);
	pxcent = viewRect.width() * percent / 100;
	viewRect.width(pxcent);
	p->getPalette(PROGRESSBAR_FG, color2);
	r->filledRectangle(viewRect, color2);
	if (showPercent)
	{
		char buffer[5];
		snprintf(buffer, sizeof(buffer), "%3d%%", percent);
		p->getPalette(PROGRESSBAR_TEXT, color2);
		r->text(viewRect, color2, color, buffer);
	}
}

void ProgressBar::handleEvent(Event *evt)
{
	View::handleEvent(evt);

	if (evt->isEventCommand())
	{
		MessageEvent *msg = evt->getMessageEvent();
		if (isCommandForMe(msg))
		{
			if (msg->command == CMD_UPDATE)
			{
				percent = (msg->subCommand > 100) ? 100 : msg->subCommand;
				setChanged(VIEW_CHANGED_REDRAW);
				/* Now ask for redrawing */
				sendCommandToTopView(CMD_REDRAW);
			}
			evt->clear();
		}
	}
}
