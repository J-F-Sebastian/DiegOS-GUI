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

#include "view.h"
#include "viewinstances.h"
#include "background_palette.h"
#include "frame_palette.h"

View::View(Rectangle &limits, unsigned char flags, View *parent) : parentView(parent),
								   topView(nullptr),
								   nextView(nullptr),
								   prevView(nullptr),
								   borders(limits),
								   extent(0, 0, limits.width() - 1, limits.height() - 1),
								   viewport(0, 0, limits.width() - 1, limits.height() - 1),
								   rflags(0),
								   sflags(VIEW_STATE_VISIBLE | VIEW_STATE_EXPOSED),
								   oflags(0),
								   cflags(VIEW_CHANGED_REDRAW),
								   aflags(flags),
								   renderBuffer(nullptr)
{
	updateViewport();
	updateRenderBuffer();
}

View::~View()
{
	parentView = nextView = prevView = nullptr;

	if (renderBuffer)
		GRenderer->releaseBuffer(renderBuffer);
}

void View::sizeLimits(Point &min, Point &max)
{
	min.x = min.y = 0;

	if (parentView)
	{
		max.x = parentView->borders.width();
		max.y = parentView->borders.height();
	}
	else
	{
		max.x = borders.width();
		max.y = borders.height();
	}
}

bool View::setLocation(const Rectangle &loc)
{
	Point min, max;

	if (getResizeMode(VIEW_ZOOMED))
		return false;

	sizeLimits(min, max);
	if ((loc.width() > max.x) ||
	    (loc.width() < min.x) ||
	    (loc.height() > max.y) ||
	    (loc.height() < min.y))
		return false;

	changeBorders(loc);
	return true;
}

bool View::moveLocation(const Point &delta)
{
	Rectangle temp(borders);
	temp.move(delta.x, delta.y);

	if (parentView && !parentView->borders.includes(temp))
		return false;

	changeBorders(temp);
	return true;
}

void View::getOrigin(Point &origin) const
{
	origin = borders.ul;
}

void View::getExtent(Rectangle &extnt)
{
	extnt = extent;
}

void View::localize(Rectangle &rect)
{
	/* Make coordinates local, relative to this view */
	/* FIXME: Should perform clipping ? */
	makeLocal(rect.ul);
	makeLocal(rect.lr);
}

void View::globalize(Rectangle &rect)
{
	/* Make coordinates global, relative to the root view */
	makeGlobal(rect.ul);
	makeGlobal(rect.lr);
}

void View::setParent(View *par)
{
	parentView = par;
}

void View::setNext(View *par)
{
	nextView = par;
}

void View::setPrev(View *par)
{
	prevView = par;
}

static const unsigned char RVALIDATE = (VIEW_BOUNDED | VIEW_ZOOMED);

void View::setResizeMode(unsigned char flags)
{
	if (flags & RVALIDATE)
		rflags |= flags;
}

bool View::getResizeMode(unsigned char flags) const
{
	if (rflags & flags)
		return true;
	else
		return false;
}

bool View::getResizeModeAll(unsigned char flags) const
{
	if ((rflags & flags) == flags)
		return true;
	else
		return false;
}

unsigned char View::getResizeMode() const
{
	return rflags;
}

void View::clearResizeMode(unsigned char flags)
{
	if (rflags & RVALIDATE)
		rflags &= ~flags;
}

void View::setOptions(unsigned char flags)
{
	if (flags & VIEW_OPT_ALL)
		oflags |= flags;
}

bool View::getOptions(unsigned char flags) const
{
	if (oflags & flags)
		return true;
	else
		return false;
}

bool View::getOptionsAll(unsigned char flags) const
{
	if ((oflags & flags) == flags)
		return true;
	else
		return false;
}

unsigned char View::getOptions() const
{
	return oflags;
}

void View::clearOptions(unsigned char flags)
{
	if (flags & VIEW_OPT_ALL)
		oflags &= ~flags;
}

static const unsigned char SVALIDATE = (VIEW_STATE_VISIBLE |
					VIEW_STATE_SELECTED |
					VIEW_STATE_FOCUSED |
					VIEW_STATE_DISABLED |
					VIEW_STATE_DRAGGING |
					VIEW_STATE_EVLOOP |
					VIEW_STATE_FOREGROUND |
					VIEW_STATE_EXPOSED);

void View::setState(unsigned char flags)
{
	if (flags & SVALIDATE)
		sflags |= flags;
}

bool View::getState(unsigned char flags) const
{
	if (sflags & flags)
		return true;
	else
		return false;
}

bool View::getStateAll(unsigned char flags) const
{
	if ((sflags & flags) == flags)
		return true;
	else
		return false;
}

unsigned char View::getState() const
{
	return sflags;
}

void View::clearState(unsigned char flags)
{
	if (flags & SVALIDATE)
		sflags &= ~flags;
}

static const unsigned char CVALIDATE = (VIEW_CHANGED_REDRAW |
					VIEW_CHANGED_DATA);

void View::setChanged(unsigned char flags)
{
	if (flags & CVALIDATE)
	{
		cflags |= flags;

		if (parentView && (cflags != parentView->cflags))
			parentView->setChanged(flags);
	}
}

bool View::getChanged(unsigned char flags) const
{
	if (cflags & flags)
		return true;
	else
		return false;
}

bool View::getChangedAll(unsigned char flags) const
{
	if ((cflags & flags) == flags)
		return true;
	else
		return false;
}

unsigned char View::getChanged() const
{
	return cflags;
}

void View::clearChanged(unsigned char flags)
{
	if (flags & CVALIDATE)
		cflags &= ~flags;
}

static const unsigned char AVALIDATE = (VIEW_IS_FRAMED |
					VIEW_IS_SHADOWED |
					VIEW_IS_SOLID);

bool View::getAttribute(unsigned char flags) const
{
	if (aflags & flags)
		return true;
	else
		return false;
}

bool View::getAttributeAll(unsigned char flags) const
{
	if ((aflags & flags) == flags)
		return true;
	else
		return false;
}

unsigned char View::getAttribute() const
{
	return aflags;
}

void View::clearAttribute(unsigned char flags)
{
	if (flags & AVALIDATE)
	{
		aflags &= ~flags;
		updateViewport();
	}
}

void View::drawView()
{
	ViewRender *r = GRenderer;

	if (aflags & VIEW_IS_SOLID)
	{
		Palette *p = GPaletteGroup->getPalette(PaletteGroup::PAL_BACKGROUND);
		unsigned color;

		if (getState(VIEW_STATE_DISABLED))
			p->getPalette(BACKGROUND_BG_DISABLED, color);
		else if (getState(VIEW_STATE_FOREGROUND))
			p->getPalette(BACKGROUND_FG, color);
		else
			p->getPalette(BACKGROUND_BG, color);

		r->filledRectangle(viewport, color);
	}

	if (aflags & VIEW_IS_FRAMED)
	{
		unsigned color[2];
		Rectangle temp(viewport);
		temp.zoom(6, 6);
		Palette *p = GPaletteGroup->getPalette(PaletteGroup::PAL_FRAME);

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

		// The frame, width 2 pixels
		p->getPalette(FRAME_MAIN, color[0]);
		temp.zoom(-1, -1);
		r->rectangle(temp, color[0]);
		temp.zoom(-1, -1);
		r->rectangle(temp, color[0]);

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
	}
	if (aflags & VIEW_IS_SHADOWED)
	{
		}
}

void View::draw()
{
	if (sflags & VIEW_STATE_EXPOSED)
	{
		Rectangle dest = extent;
		makeGlobal(dest.ul);
		makeGlobal(dest.lr);
		GRenderer->writeBuffer(renderBuffer, extent, dest);
	}
}

void View::reDraw()
{
	if (getChanged(VIEW_CHANGED_REDRAW))
	{
		GRenderer->setBuffer(renderBuffer);
		drawView();
		clearChanged(VIEW_CHANGED_REDRAW);
	}
}

void View::handleEvent(Event *evt)
{
	/*
	 * Mouse or touch event
	 */
	if (isEventPositional(evt))
	{
		/*
		 * Pressure detected and view NOT focused yet
		 */
		if (evt->testPositionalEventStatus(POS_EVT_PRESSED) && !getState(VIEW_STATE_FOCUSED))
		{
			if (parentView)
			{
				if (parentView->executeCommand(CMD_REQ_FOCUS, this))
					if (focus())
						sendCommand(CMD_REDRAW);
			}
			else
			{
				if (focus())
					sendCommand(CMD_REDRAW);
			}
		}
	}
	else if (evt->isEventCommand())
	{
		MessageEvent *me = evt->getMessageEvent();
		if (isCommandForMe(me))
		{
			if (executeCommand(me->command /*, reinterpret_cast<View *>(me->senderObject)*/))
				evt->clear();
		}
	}
}

bool View::executeCommand(const uint16_t command, View *caller)
{
	if (validateCommand(command))
	{
		switch (command)
		{
		case CMD_DRAW:
			draw();
			return true;

		case CMD_REDRAW:
			reDraw();
			return true;

		case CMD_REL_FOCUS:
			clearState(VIEW_STATE_FOCUSED | VIEW_STATE_SELECTED);
			return true;

		case CMD_QUIT:
			// FIXME
			return true;
		}
	}

	return false;
}

bool View::validateCommand(const uint16_t command)
{
	switch (command)
	{
	case CMD_DRAW:
	/* FALLTHRU */
	case CMD_REDRAW:
	/* FALLTHRU */
	case CMD_REL_FOCUS:
	/* FALLTHRU */
	case CMD_QUIT:
		return true;
	}

	return false;
}

void View::sendCommand(const uint16_t command, void *destination, void *target)
{
	Event evt;
	MessageEvent cmd = {command, 0, this, destination, target, {0, 0, 0, 0}};
	if (destination && target)
	{
		evt.setMessageEvent(cmd);
		sendEvent(&evt);
	}
}

void View::sendCommandToParent(const uint16_t command)
{
	Event evt;
	MessageEvent cmd = {command, 0, this, parentView, this, {0, 0, 0, 0}};
	evt.setMessageEvent(cmd);
	sendEvent(&evt);
}

void View::computeExposure()
{
	Rectangle temp(extent);
	globalize(temp);

	setExposed(!GZBuffer->isAreaSet(temp));

	if (getState(VIEW_STATE_EXPOSED))
		GZBuffer->set(temp);
}

void View::sendEvent(Event *evt)
{
	if (isCommandForMe(evt->getMessageEvent()))
		handleEvent(evt);
	else if (getTopView())
		getTopView()->sendEvent(evt);
}

bool View::isEventPositional(Event *evt)
{
	/* If the event is positional return true */
	return (evt && evt->isEventPositional()) ? true : false;
}

bool View::isEventPositionValid(Event *evt)
{
	return isEventPositional(evt) && isEventPositionInRange(evt);
}

bool View::isEventPositionInRange(Event *evt)
{
	Rectangle lims;
	Point where(evt->getPositionalEvent()->x, evt->getPositionalEvent()->y);

	makeLocal(where);
	getExtent(lims);

	// lims.print();
	// where.print();
	/* if the event coordinates fall outside our limits then return false */
	return lims.includes(where);
}

bool View::isCommandForMe(MessageEvent *evt)
{
	if (evt->destObject == this)
		return true;

	return false;
}

bool View::isCommandTargetMe(MessageEvent *evt)
{
	if (evt->targetObject == this)
		return true;

	return false;
}

bool View::isCommandForAll(MessageEvent *evt)
{
	if (evt->destObject == BROADCAST_OBJECT)
		return true;

	return false;
}

bool View::isCommandTargetAll(MessageEvent *evt)
{
	if (evt->targetObject == BROADCAST_OBJECT)
		return true;

	return false;
}

bool View::isCommandMe(MessageEvent *evt)
{
	if ((evt->destObject == this) && (evt->targetObject == this))
		return true;

	return false;
}

bool View::isCommandAny(MessageEvent *evt)
{
	if ((evt->destObject == BROADCAST_OBJECT) && (evt->targetObject == BROADCAST_OBJECT))
		return true;

	return false;
}

bool View::focus()
{
	if (getState(VIEW_STATE_FOCUSED))
		return true;

	if (!getState(VIEW_STATE_EVLOOP))
	{
		if (select())
		{
			setState(VIEW_STATE_FOCUSED);
			return true;
		}
	}

	return false;
}

bool View::select()
{
	if (!getOptions(VIEW_OPT_SELECTABLE))
		return false;

	if (parentView)
	{
		if (!parentView->executeCommand(CMD_SELECT, this))
			return false;
	}
	setState(VIEW_STATE_SELECTED);

	return true;
}

void View::setBorders(const Rectangle &newrect)
{
	if (borders != newrect)
	{
		borders = newrect;

		// View was resized
		if (!borders.superpose(extent))
		{
			extent.lr = Point(borders.width() - 1, borders.height() - 1);
			updateViewport();
			updateRenderBuffer();
		}
		setChanged(VIEW_CHANGED_REDRAW);
	}
}

void View::getBorders(Rectangle &rect)
{
	rect = borders;
}

void View::calcLimits(const Point &delta, Rectangle &newrect)
{
	newrect = borders;
	if (rflags & VIEW_RESIZE_UX)
		newrect.ul.x += delta.x;
	if (rflags & VIEW_RESIZE_UY)
		newrect.ul.y += delta.y;
	if (rflags & VIEW_RESIZE_LX)
		newrect.lr.x += delta.x;
	if (rflags & VIEW_RESIZE_LY)
		newrect.lr.y += delta.y;
}

void View::changeBorders(const Rectangle &newrect)
{
	setBorders(newrect);
}

void View::makeLocal(Point &origin)
{
	if (parentView)
	{
		parentView->makeLocal(origin);
	}
	origin -= borders.ul;
}

void View::makeGlobal(Point &origin)
{
	if (parentView)
	{
		parentView->makeGlobal(origin);
	}
	origin += borders.ul;
}

void View::setExposed(bool exposed)
{
	if (getState(VIEW_STATE_VISIBLE) && exposed)
	{
		setState(VIEW_STATE_EXPOSED);
	}
	else
	{
		clearState(VIEW_STATE_EXPOSED);
	}
}

void View::setForeground()
{
	setState(VIEW_STATE_FOREGROUND);
	setChanged(VIEW_CHANGED_REDRAW);
}

void View::setBackground()
{
	clearState(VIEW_STATE_FOREGROUND);
	setChanged(VIEW_CHANGED_REDRAW);
}

View *View::getTopView()
{
	if (parentView == nullptr)
		return this;

	if (topView == nullptr)
	{
		topView = parentView;
		while (topView->parentView && !topView->getState(VIEW_STATE_EVLOOP))
		{
			topView = topView->parentView;
		}
	}

	return topView;
}

void View::updateRenderBuffer()
{
	if (renderBuffer)
		GRenderer->releaseBuffer(renderBuffer);

	renderBuffer = GRenderer->createBuffer(extent);
}

void View::updateViewport()
{
	viewport = extent;
	if (aflags & VIEW_IS_FRAMED)
		viewport.zoom(-6, -6);
	if (aflags & VIEW_IS_SHADOWED)
		viewport.lr.move(-2, -1);
}

void View::getViewport(Rectangle &rect)
{
	rect = viewport;
}