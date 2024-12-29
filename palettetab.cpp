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
#include "palettetab.h"

PaletteTab::PaletteTab(Rectangle &viewLimits) : View(viewLimits)
{
	clearOptions(VIEW_OPT_VALIDATE);
}

void PaletteTab::drawView()
{
	Rectangle viewRect;
	ViewRender *r = GRenderer;
	Palette *p = GSystemPalette;
	getViewport(viewRect);
	unsigned ruled = p->size() / 2;

	viewRect.width(viewRect.width() / ruled);
	viewRect.height(viewRect.height() / 2);
	unsigned color;

	for (unsigned i = 0; i < ruled; i++)
	{
		p->getPalette(i, color);
		r->filledRectangle(viewRect, color);
		viewRect.move(viewRect.width(), 0);
	}
	viewRect.move(0, viewRect.height());
	for (unsigned i = ruled; i < p->size(); i++)
	{
		p->getPalette(i, color);
		r->filledRectangle(viewRect, color);
		viewRect.move(-viewRect.width(), 0);
	}
}

void PaletteTab::handleEvent(Event *evt)
{
	static int p = 0;

	View::handleEvent(evt);

	if (isEventPositionValid(evt))
	{
		if (evt->testPositionalEventStatus(POS_EVT_SINGLE))
		{
			SystemPaletteInstance::instance()->configure((SystemColorPalette)p, 32);
			PaletteGroupInstance::instance()->configure((SystemColorPalette)p, 32);
			p++;
			if (p == SYS_PALETTE_NUM)
				p = 0;

			setChanged(VIEW_CHANGED_REDRAW);
			sendCommandToTopView(CMD_REDRAW);
		}
	}
}