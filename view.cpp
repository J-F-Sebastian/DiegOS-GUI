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

View::View(Rectangle &limits, ViewRender *rnd, Palette *pal, View *parent) : parentView(parent), renderer(rnd), palette(pal), borders(limits), extent(0, 0, limits.width() - 1, limits.height() - 1), rflags(0), sflags(VIEW_STATE_VISIBLE), oflags(0), cflags(VIEW_CHANGED_REDRAW)
{
}

View::~View()
{
	parentView = nullptr;
	renderer = nullptr;
	palette = nullptr;
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

void View::getExtent(Rectangle &extent)
{
	extent = this->extent;
}

void View::localize(Rectangle &rect)
{
	Point p = rect.ul;

	/* Make coordinates local, relative to this view */
	makeLocal(p);
	rect.lr -= rect.ul;
	rect.ul = p;
	rect.lr += p;
	/* FIXME: Should perform clipping ? */
}

void View::globalize(Rectangle &rect)
{
	Point p = rect.ul;

	/* Make coordinates global, relative to the root view */
	makeGlobal(p);
	rect.lr -= rect.ul;
	rect.ul = p;
	rect.lr += p;
}

void View::setParent(View *par)
{
	if (!parentView && par)
	{
		parentView = par;
	}
}

void View::setRenderer(ViewRender *rnd)
{
	if (!renderer && rnd)
	{
		renderer = rnd;
	}
}

void View::setPalette(Palette *pal)
{
	if (pal && !palette)
	{
		palette = pal;
	}
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
		cflags |= flags;
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

void View::reDraw()
{
	if (getChanged(VIEW_CHANGED_REDRAW))
	{
		draw();
		clearChanged(VIEW_CHANGED_REDRAW);
	}
}

void View::handleEvent(Event *evt)
{
	if (isEventPositionValid(evt))
	{
		if (evt->testPositionalEventStatus(POS_EVT_PRESSED | POS_EVT_DOUBLE))
		{
			select();
		}
	}
}

void View::sendCommand(const uint16_t command, void *destination, void *target)
{
	Event evt;
	MessageEvent cmd = {command, 0, this, destination, target, nullptr};
	evt.setMessageEvent(cmd);
	sendEvent(&evt);
}

void View::sendCommand(const uint16_t command)
{
	Event evt;
	MessageEvent cmd = {command, 0, this, BROADCAST_OBJECT, this, nullptr};
	evt.setMessageEvent(cmd);
	sendEvent(&evt);
}

void View::sendEvent(Event *evt)
{
	if (getParent())
		getParent()->sendEvent(evt);
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

	/* if the event coordinates fall outside our limits then return false */
	return lims.includes(where);
}

bool View::isEventCmdForMe(Event *evt)
{
	if (evt && evt->getEventType() == Event::EVT_CMD)
		return (evt->getMessageEvent()->destObject == this) ? true : false;
	return false;
}

static const unsigned char FOCUSVAL = (VIEW_STATE_SELECTED | VIEW_STATE_EVLOOP);

bool View::focus()
{
	if (!getOptions(VIEW_OPT_SELECTABLE))
		return false;

	bool checkflags = !getState(VIEW_STATE_SELECTED | VIEW_STATE_EVLOOP);
	if ((getParent() == nullptr) || (checkflags && getParent()->focus()))
	{
		select();
		setState(VIEW_STATE_FOCUSED);
		return true;
	}

	return false;
}

void View::select()
{
	if (!getOptions(VIEW_OPT_SELECTABLE) || getState(VIEW_STATE_SELECTED))
		return;

	setState(VIEW_STATE_SELECTED);

	if (!getOptions(VIEW_OPT_TOPSELECT))
		return;

	if (getParent())
	{
		if (getParent()->getState(VIEW_STATE_FOCUSED))
			setState(VIEW_STATE_FOCUSED);

		sendCommand(CMD_SELECT, getParent(), this);
	}
}

void View::setBorders(const Rectangle &newrect)
{
	borders = newrect;
	extent.lr = Point(borders.width() - 1, borders.height() - 1);
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

void View::clearExposed(Rectangle &covered)
{
	if (covered.includes(borders))
	{
		std::cout << covered.ul.x << "," << covered.ul.y << " " << covered.lr.x << "," << covered.lr.y << std::endl;
		setExposed(false);
	}
}

void View::setForeground()
{
	setState(VIEW_STATE_FOREGROUND);
}

void View::setBackground()
{
	clearState(VIEW_STATE_FOREGROUND);
}