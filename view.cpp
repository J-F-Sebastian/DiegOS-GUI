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
View::View(Rectangle &limits, View *parent) : parentView(parent),

					      nextView(nullptr),
					      borders(limits),
					      extent(0, 0, limits.width() - 1, limits.height() - 1),
					      rflags(0),
					      sflags(VIEW_STATE_VISIBLE | VIEW_STATE_EXPOSED),
					      oflags(0),
					      cflags(VIEW_CHANGED_REDRAW)
{
}

View::~View()
{
	parentView = nullptr;
	nextView = nullptr;
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
	parentView = par;
}

void View::setNext(View *par)
{
	nextView = par;
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
		if (/*!getState(VIEW_STATE_EVLOOP) && */ getParent())
			getParent()->setChanged(flags);
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

void View::draw()
{
	Rectangle area;
	Rectangle dest;

	if (getParent() == nullptr)
	{
		area = extent;
		dest = extent;
	}
	else
	{
		area = borders;
		dest = borders;
		makeGlobal(dest.ul);
		makeGlobal(dest.lr);
	}

	GRenderer->writeBuffer(renderBuffer, area, dest);
}

void View::reDraw()
{
	if (getChanged(VIEW_CHANGED_REDRAW))
	{
		GRenderer->setBuffer(renderBuffer);
		std::cout << std::hex << this << "  " << __FUNCSIG__ << std::endl;
		drawView();
		draw();
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
			if (getParent())
			{
				if (getParent()->executeCommand(CMD_REQ_FOCUS, this))
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
	MessageEvent cmd = {command, 0, this, getParent(), this, {0, 0, 0, 0}};
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
	else if (topView())
		topView()->sendEvent(evt);
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
			// sendCommand(CMD_DRAW);
			return true;
		}
	}

	return false;
}

bool View::select()
{
	if (!getOptions(VIEW_OPT_SELECTABLE))
		return false;

	if (getParent())
	{
		if (!getParent()->executeCommand(CMD_SELECT, this))
			return false;
	}
	setState(VIEW_STATE_SELECTED);

	return true;
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

void View::setForeground()
{
	setState(VIEW_STATE_FOREGROUND);
}

void View::setBackground()
{
	clearState(VIEW_STATE_FOREGROUND);
}

View *View::topView()
{
	if (parentView == nullptr)
		return this;

	View *top = parentView;
	while (top->parentView && !top->getState(VIEW_STATE_EVLOOP))
	{
		top = top->parentView;
	}

	return top;
}