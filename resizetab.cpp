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

#include "viewinstances.h"
#include "resizetab.h"
#include "window_icon_palette.h"

ResizeTab::ResizeTab(Rectangle &viewLimits) : View(viewLimits), resize(0, 0)
{
	setResizeMode(VIEW_BOUNDED);
	clearOptions(VIEW_OPT_VALIDATE);
}

void ResizeTab::drawView()
{
	Rectangle viewRect;
	ViewRender *r = GRenderer;
	Palette *p = GPaletteGroup->getPalette(PaletteGroup::PAL_WINICON);
	getViewport(viewRect);

	unsigned color[2];
	p->getPalette(WINICON_BRIGHT, color[0]);
	p->getPalette(WINICON_DARK, color[1]);

	// Outer shadow

	/*
	 *    BBBBBBBBB
	 *    B       D
	 *    B       D
	 *    B       D
	 *    DDDDDDDDD
	 */
	r->frame(viewRect, 2, color, false);

	if (getState(VIEW_STATE_DISABLED))
		p->getPalette(WINICON_DISABLED, color[0]);
	else if (getParent() && getParent()->getState(VIEW_STATE_DRAGGING))
		p->getPalette(WINICON_DRAGGING, color[0]);
	else
		p->getPalette(WINICON_MAIN, color[0]);

	viewRect.zoom(-2, -2);
	r->filledRectangle(viewRect, color[0]);
}

void ResizeTab::handleEvent(Event *evt)
{
	View::handleEvent(evt);

	if (isEventPositionValid(evt))
	{
		if (evt->testPositionalEventStatus(POS_EVT_DRAG))
		{
			bool pressed = (evt->testPositionalEventStatus(POS_EVT_PRESSED) && evt->testPositionalEventPos(POS_EVT_LEFT));

			if (pressed)
			{
				if (!getParent()->getState(VIEW_STATE_DRAGGING))
				{
					getParent()->setState(VIEW_STATE_DRAGGING);
				}

				Rectangle rect;
				getParent()->getBorders(rect);
				rect.lr.move(evt->getPositionalEvent()->xrel, evt->getPositionalEvent()->yrel);
				getParent()->setLocation(rect);
				setChanged(VIEW_CHANGED_REDRAW);
				/* Now ask for redrawing */
				sendCommandToTopView(CMD_REDRAW);
			}
		}
		else if (getParent()->getState(VIEW_STATE_DRAGGING))
		{
			/* Reset dragging */
			getParent()->clearState(VIEW_STATE_DRAGGING);
			setChanged(VIEW_CHANGED_REDRAW);
			/* Now ask for redrawing */
			sendCommandToTopView(CMD_REDRAW);
		}
	}
}