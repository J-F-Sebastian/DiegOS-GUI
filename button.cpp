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
#include "button.h"
#include "button_palette.h"

Button::Button(Rectangle &rect) : AbstractButton(rect)
{
}

void Button::drawView()
{
	Rectangle viewRect;
	ViewRender *r = GRenderer;
	Palette *p = GPaletteGroup->getPalette(PaletteGroup::PAL_BUTTON);
	unsigned color[2];

	getViewport(viewRect);
	p->getPalette(BUTTON_BRIGHT, color[0]);
	p->getPalette(BUTTON_DARK, color[1]);

	// Outer shadow

	/*
	 *    BBBBBBBBB
	 *    B       D
	 *    B       D
	 *    B       D
	 *    DDDDDDDDD
	 */
	r->frame(viewRect, 2, color, false);

	// Inner shadow

	/*
	 *    DDDDDDDDD
	 *    D       B
	 *    D       B
	 *    D       B
	 *    BBBBBBBBB
	 */
	viewRect.zoom(-2, -2);
	r->frame(viewRect, 1, color, true);

	// Outer shadow

	/*
	 *    BBBBBBBBB
	 *    B       D
	 *    B       D
	 *    B       D
	 *    DDDDDDDDD
	 */
	viewRect.zoom(-1, -1);
	r->frame(viewRect, 1, color, false);

	viewRect.zoom(-1, -1);
	if (getState(VIEW_STATE_DISABLED))
	{
		p->getPalette(BUTTON_DISABLED, color[0]);
		r->filledRectangle(viewRect, color[0]);
	}
	else if (getState(VIEW_STATE_SELECTED))
	{
		if (isDown())
		{
			p->getPalette(BUTTON_PRESSED, color[0]);
		}
		else
		{
			p->getPalette(BUTTON_SELECTED, color[0]);
		}
		r->filledRectangle(viewRect, color[0]);
	}
	else
	{
		p->getPalette(BUTTON_MAIN, color[0]);
		r->filledRectangle(viewRect, color[0]);
	}
}

void Button::doAction()
{
}