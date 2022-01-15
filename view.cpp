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

View::View(Rectangle &limits, ViewRender *rnd, Palette *pal, View *parent) : parentView(parent), renderer(rnd), palette(pal), limits(limits), extent(0, 0, limits.width() - 1, limits.height() - 1), rflags(0), sflags(VIEW_STATE_VISIBLE), oflags(0)
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
		max.x = parentView->limits.width();
		max.y = parentView->limits.height();
	}
	else
	{
		max.x = limits.width();
		max.y = limits.height();
	}
}

bool View::setLocation(const Rectangle &loc)
{
	Point min, max;
	sizeLimits(min, max);
	if ((loc.width() > max.x) ||
	    (loc.width() < min.x) ||
	    (loc.height() > max.y) ||
	    (loc.height() < min.y))
		return false;

	changeLimits(loc);
	return true;
}

bool View::moveLocation(const Point &delta)
{
	Rectangle temp(limits);
	temp.move(delta.x, delta.y);

	if (parentView)
	{
		if (!parentView->limits.includes(temp))
		{
			std::cout << "ERR" << std::endl;
			return false;
		}
	}

	changeLimits(temp);
	return true;
}

void View::getOrigin(Point &origin) const
{
	origin = limits.ul;
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

static const unsigned char RVALIDATE = VIEW_RESIZEABLE;

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
					VIEW_STATE_EVLOOP);

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

void View::clearState(unsigned char flags)
{
	if (flags & SVALIDATE)
		sflags &= ~flags;
}

void View::handleEvent(Event *evt)
{
	if (isEventPositionValid(evt))
	{
		MouseEvent *mouse = evt->getMouseEvent();
		if (mouse->pressed)
		{
			if (!getState(VIEW_STATE_SELECTED | VIEW_STATE_DISABLED) && getOptions(VIEW_OPT_SELECTABLE))
			{
				select();
			}
		}
	}
}

bool View::isEventPositionValid(Event *evt)
{
	Rectangle lims;

	if (!evt)
		return false;

	/* If the event is not positional return false */
	if (!evt->isEventPositional())
		return false;

	MouseEvent *mouse = evt->getMouseEvent();
	Point where(mouse->x, mouse->y);
	getExtent(lims);
	globalize(lims);
	/* if it falls outside our limits then return false */

	return lims.includes(where);
}

static const unsigned char FOCUSVAL = (VIEW_STATE_SELECTED | VIEW_STATE_EVLOOP);

bool View::focus()
{
	if ((oflags & VIEW_OPT_SELECTABLE) == 0)
		return false;

	bool checkflags = !getState(VIEW_STATE_SELECTED | VIEW_STATE_EVLOOP);
	if ((parentView == nullptr) || (checkflags && parentView->focus()))
	{
		select();
		sflags |= VIEW_STATE_FOCUSED;
		return true;
	}

	return false;
}

void View::select()
{
	Event evt;
	MessageEvent cmd;

	if (!getOptions(VIEW_OPT_SELECTABLE))
		return;

	setState(VIEW_STATE_SELECTED);
	if (parentView)
	{
		if (parentView->getState(VIEW_STATE_FOCUSED))
			setState(VIEW_STATE_FOCUSED);

		cmd.senderObject = this;
		cmd.destObject = parentView;
		cmd.targetObject = this;
		if (getOptions(VIEW_OPT_TOPSELECT))
		{
			cmd.command = CMD_FOREGROUND;
		}
		else
		{
			cmd.command = CMD_SELECT;
		}
		evt.setMessageEvent(cmd);
		parentView->handleEvent(&evt);
	}
}

void View::setLimits(const Rectangle &newrect)
{
	limits = newrect;
	extent.lr = Point(limits.width() - 1, limits.height() - 1);
}

void View::getLimits(Rectangle &rect)
{
	rect = limits;
}

void View::calcLimits(const Point &delta, Rectangle &newrect)
{
	newrect = limits;
	if (rflags & VIEW_RESIZE_UX)
		newrect.ul.x += delta.x;
	if (rflags & VIEW_RESIZE_UY)
		newrect.ul.y += delta.y;
	if (rflags & VIEW_RESIZE_LX)
		newrect.lr.x += delta.x;
	if (rflags & VIEW_RESIZE_LY)
		newrect.lr.y += delta.y;
}

void View::changeLimits(const Rectangle &newrect)
{
	setLimits(newrect);
	draw();
}

void View::makeLocal(Point &origin)
{
	if (parentView)
	{
		parentView->makeLocal(origin);
	}
	origin -= limits.ul;
}

void View::makeGlobal(Point &origin)
{
	if (parentView)
	{
		parentView->makeGlobal(origin);
	}
	origin += limits.ul;
}

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

ViewGroup::ViewGroup(Rectangle &limits, ViewRender *rnd, PaletteGroup *pals, View *parent) : View(limits, rnd, nullptr, parent), lastLimits(limits), focused(nullptr), selected(nullptr), palettes(pals), viewList()
{
	setOptions(VIEW_OPT_SELECTABLE);
}

ViewGroup::~ViewGroup()
{
	List<View *>::iterator it = viewList.begin();
	while (it != viewList.end())
	{
		delete (*it);
		it++;
	}
	viewList.clear();
}

bool ViewGroup::setLocation(const Rectangle &loc)
{
	Rectangle update;
	Point delta(0, 0);
	Point min, max;

	sizeLimits(min, max);
	if ((loc.width() > max.x) ||
	    (loc.width() < min.x) ||
	    (loc.height() > max.y) ||
	    (loc.height() < min.y))
		return false;

	getLimits(update);
	delta.x = loc.width() - update.width();
	delta.y = loc.height() - update.height();
	setLimits(loc);

	// No parenting means this is the root object, the main container of all graphics
	if (!parentView)
		renderer->clear(0);

	List<View *>::iterator it = viewList.begin();
	while (it != viewList.end())
	{
		(*it)->calcLimits(delta, update);
		(*it)->setLocation(update);
		it++;
	}

	// No parenting means this is the root object, the main container of all graphics
	if (!parentView)
		renderer->show();

	return true;
}

bool ViewGroup::moveLocation(const Point &delta)
{
	if (View::moveLocation(delta))
	{
		return true;
	}
	return false;
}

void ViewGroup::draw()
{
	// No parenting means this is the root object, the main container of all graphics
	if (!parentView)
		renderer->clear(0);

	// Draw children back-to-top, following the painter algorithm
	for (List<View *>::riterator it = viewList.rbegin(); it != viewList.rend(); it++)
	{
		(*it)->draw();
	}
	// No parenting means this is the root object, the main container of all graphics
	if (!parentView)
		renderer->show();
}

void ViewGroup::handleEvent(Event *evt)
{
	View::handleEvent(evt);

	/* If the event is positional, look for a view including the event in its
	 * boundaries; first view accepting the coordinates of the event will process the
	 * event with handleEvent() method.
	 */
	if (evt->isEventPositional())
	{
		List<View *>::iterator it;
		for (it = viewList.begin(); it != viewList.end(); it++)
		{
			if ((*it)->isEventPositionValid(evt))
				break;
		}

		if (it != viewList.end())
			(*it)->handleEvent(evt);
	}
	/*
	 * If the event is a message (a command) then the event is processed by either the
	 * target view (if specified) or all views (if the target is the broadcast address).
	 */
	else if (evt->getEventType() == Event::EVT_CMD)
	{
		MessageEvent *msg = evt->getMessageEvent();
		if ((msg->destObject == this) || (msg->destObject == BROADCAST_OBJECT))
		{
			switch (msg->command)
			{
			case CMD_FOREGROUND:
				std::cout << "Foreground" << std::endl;
				toTheTop(static_cast<View *>(msg->targetObject));
				break;
			case CMD_SELECT:
				std::cout << "Select" << std::endl;
				selectView(static_cast<View *>(msg->targetObject));
				if (selected->getOptions(VIEW_OPT_TOPSELECT))
					toTheTop(selected);
				break;
			}
			if (msg->destObject == this)
				return;
		}

		for (List<View *>::iterator it = viewList.begin(); it != viewList.end(); it++)
		{
			if ((msg->destObject == (*it)) || (msg->destObject == BROADCAST_OBJECT))
			{
				(*it)->handleEvent(evt);
			}
		}
	}
	/*
	 * Other events are processed by all views.
	 */
	else
	{
		for (List<View *>::iterator it = viewList.begin(); it != viewList.end(); it++)
			(*it)->handleEvent(evt);
	}
}

void ViewGroup::insert(View *newView)
{
	if (newView)
	{
		newView->setParent(this);
		newView->setRenderer(renderer);
		if (newView->getOptions(VIEW_OPT_TOPSELECT))
		{
			viewList.addHead(newView);
			if (selected)
			{
				selected->clearState(VIEW_STATE_SELECTED);
			}
			newView->setState(VIEW_STATE_SELECTED);
			selected = newView;
		}
		else
		{
			viewList.addTail(newView);
		}
	}
}

void ViewGroup::setPalettes(PaletteGroup *pals)
{
	if (pals && !palettes)
	{
		palettes = pals;
	}
}

bool ViewGroup::focusNext(bool forward)
{
	View *temp = nullptr;

	if (viewList.count() < 1)
		return false;

	// FIXME missing check if focus can be handed off
	if (forward)
	{
		List<View *>::iterator it(viewList, &focused);

		if (++it == viewList.end())
			it = viewList.begin();

		temp = (*it);
	}
	else
	{
		List<View *>::riterator it(viewList, &focused);

		if (++it == viewList.rend())
			it = viewList.rbegin();

		temp = (*it);
	}

	return focusView(temp);
}

void ViewGroup::selectNext(bool forward)
{
	View *temp = nullptr;

	if (viewList.count() < 1)
		return;

	if (forward)
	{
		List<View *>::iterator it(viewList, &selected);

		if (++it == viewList.end())
			it = viewList.begin();

		temp = (*it);
	}
	else
	{
		List<View *>::riterator it(viewList, &selected);

		if (++it == viewList.rend())
			it = viewList.rbegin();

		temp = (*it);
	}

	selectView(temp);
}

bool ViewGroup::focusView(View *target)
{
	if (!target)
		return false;

	if (focused)
	{
		focused->clearState(VIEW_STATE_FOCUSED);
	}
	target->setState(VIEW_STATE_FOCUSED);
	focused = target;

	return true;
}

void ViewGroup::selectView(View *target)
{
	if (!target)
		return;

	if (selected)
	{
		selected->clearState(VIEW_STATE_SELECTED);
	}
	target->setState(VIEW_STATE_SELECTED);
	selected = target;
}

void ViewGroup::toTheTop(View *target)
{
	if (viewList.count() < 1)
		return;

	List<View *>::iterator it(viewList, &target);

	if (it != viewList.end())
	{
		it = viewList.erase(it);
		std::cout << "count " << viewList.count() << std::endl;
		viewList.addHead(target);
		std::cout << "count " << viewList.count() << std::endl;
	}
}

void ViewGroup::maximize()
{
	Rectangle max;
	getLimits(max);
	setLocation(max);
}

void ViewGroup::minimize()
{
	setLocation(lastLimits);
}