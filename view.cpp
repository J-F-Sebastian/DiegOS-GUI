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

View::View(Rectangle &limits, ViewRender *rnd, Palette *pal, View *parent) : parentView(parent), renderer(rnd), palette(pal), borders(limits), extent(0, 0, limits.width() - 1, limits.height() - 1), rflags(0), sflags(VIEW_STATE_VISIBLE), oflags(0)
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
		if (evt->testPositionalEventStatus(POS_EVT_PRESSED))
		{
			if (!getState(VIEW_STATE_SELECTED | VIEW_STATE_DISABLED) && getOptions(VIEW_OPT_SELECTABLE))
			{
				select();
			}
		}
	}
}

void View::sendEvent(Event *evt)
{
	if (parentView)
		parentView->sendEvent(evt);
}

bool View::isEventPositionValid(Event *evt)
{
	if (!evt)
		return false;

	/* If the event is not positional return false */
	if (!evt->isEventPositional())
		return false;

	return isEventPositionInRange(evt);
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

	if (!getOptions(VIEW_OPT_SELECTABLE) || getState(VIEW_STATE_SELECTED))
		return;

	std::cout << __func__ << reinterpret_cast<intptr_t>(this) << std::endl;

	setState(VIEW_STATE_SELECTED);

	if (!getOptions(VIEW_OPT_TOPSELECT))
		return;

	if (parentView)
	{
		if (parentView->getState(VIEW_STATE_FOCUSED))
			setState(VIEW_STATE_FOCUSED);

		cmd.senderObject = this;
		cmd.destObject = parentView;
		cmd.targetObject = this;
		cmd.command = CMD_FOREGROUND;
		evt.setMessageEvent(cmd);
		sendEvent(&evt);
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
	setChanged(VIEW_CHANGED_REDRAW);
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

	getBorders(update);
	delta.x = loc.width() - update.width();
	delta.y = loc.height() - update.height();
	// No parenting means this is the root object, the main container of all graphics
	if (!parentView)
		renderer->clear(0);
	setBorders(loc);

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

void ViewGroup::reDraw()
{
	if (getState(VIEW_CHANGED_REDRAW))
	{
		// No parenting means this is the root object, the main container of all graphics
		if ((getState(VIEW_STATE_EVLOOP)))
			getRenderer()->clear(0);

		// Draw children back-to-top, following the painter algorithm
		for (List<View *>::riterator it = viewList.rbegin(); it != viewList.rend(); it++)
		{
			(*it)->reDraw();
		}
		clearChanged(VIEW_CHANGED_REDRAW);
		// No parenting means this is the root object, the main container of all graphics
		if ((getState(VIEW_STATE_EVLOOP)))
			getRenderer()->show();
	}
}

void ViewGroup::handleEvent(Event *evt)
{
	View::handleEvent(evt);

	/* If the event is positional, look for a view including the event in its
	 * boundaries; first view accepting the coordinates of the event will process the
	 * event with handleEvent() method.
	 */
	if (isEventPositionValid(evt))
	{
		List<View *>::iterator it;
		for (it = viewList.begin(); it != viewList.end(); it++)
		{
			(*it)->handleEvent(evt);
		}
		evt->clear();
	}
	/*
	 * If the event is a message (a command) then the event is processed if the destination
	 * is either this view or the broadcast object.
	 * The event will be propagated to children if the destination is the broadcast object
	 * or is NOT this view.
	 *
	 * +---------------+----------+
	 * |  destination  |  action  |
	 * +---------------+----------+
	 * |               |  execute |
	 * |  this view    |    and   |
	 * |               |   clear  |
	 * +---------------+----------+
	 * |               |  execute |
	 * |  broadcast    |    and   |
	 * |               |  forward |
	 * +---------------+----------+
	 *
	 * The targetObject is part of the header message but is used to address the action
	 * of the command, not the destination.
	 * A broadcast destination will make a message be processed by all handleEvent methods
	 * of all Views in the system.
	 * A unicast destination will make a message be processed by handleEvent methods
	 * until the destination view is reached; the message will be forwarded from root to leaves
	 * until the destination leave (a View) is reached.
	 */
	else if (evt->getEventType() == Event::EVT_CMD)
	{
		MessageEvent *msg = evt->getMessageEvent();
		if (msg->destObject == this)
		{
			switch (msg->command)
			{
			case CMD_FOREGROUND:
				std::cout << "Foreground CMD " << reinterpret_cast<intptr_t>(msg->targetObject) << std::endl;
				toTheTop(static_cast<View *>(msg->targetObject));
				/* FALLTHRU */
			case CMD_SELECT:
				std::cout << "Select CMD " << reinterpret_cast<intptr_t>(msg->targetObject) << std::endl;
				selectView(static_cast<View *>(msg->targetObject));
				break;

			case CMD_CLOSE:
				std::cout << "CLOSE CMD " << reinterpret_cast<intptr_t>(msg->targetObject) << std::endl;
				/* We are instructed to close, so send and event to our owner */
				if (msg->targetObject == this)
				{
					sendCommand(CMD_CLOSE, getParent(), this);
				}
				/* We are instructed to close one of our views */
				else if (msg->senderObject == msg->targetObject)
				{
					View *target = reinterpret_cast<View *>(msg->targetObject);
					if (target)
					{
						List<View *>::iterator it(viewList, &target);
						if (it != viewList.end())
						{
							// FIXME THIS IS NOT GONNA WORK AS EXPECTED...
							if (target->getState(VIEW_STATE_FOCUSED))
								focusNext(true);
							if (target->getState(VIEW_STATE_SELECTED))
								selectNext(true);
							viewList.erase(it);
							delete target;
							/* Now ask for redrawing */
							sendCommand(CMD_DRAW);
						}
					}
				}
				break;

			case CMD_MAXIMIZE:
				maximize();
				break;

			case CMD_RESTORE:
				minimize();
				break;

			case CMD_DRAW:
			{
				View *target = static_cast<View *>(msg->targetObject);
				List<View *>::iterator it(viewList, &target);
				if (it != viewList.end())
					(*it)->draw();
				else
					draw();
			}
			break;

			case CMD_REDRAW:
				reDraw();
				break;
			}
			evt->clear();
		}
		else
		{
			/*
			 * Process BROADCAST_OBJECT and leave the message as is.
			 */
			if (msg->destObject == BROADCAST_OBJECT)
			{
				switch (msg->command)
				{
				case CMD_DRAW:
					draw();
					break;
				}
			}
			/*
			 * Forward messages.
			 */
			for (List<View *>::iterator it = viewList.begin(); it != viewList.end(); it++)
			{
				(*it)->handleEvent(evt);
			}
		}
	}
	/*
	 * FIXME: Other events are processed by all views and applied by the focused view.
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
		newView->setRenderer(getRenderer());
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

void ViewGroup::insertBefore(View *newView, View *target)
{
	if (newView && target)
	{
		if (viewList.insert(newView, target))
		{
			newView->setParent(this);
			newView->setRenderer(getRenderer());
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

		if (++it != viewList.end())
			temp = (*it);
	}
	else
	{
		List<View *>::riterator it(viewList, &focused);

		if (++it != viewList.rend())
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

		if (++it != viewList.end())
			temp = (*it);
	}
	else
	{
		List<View *>::riterator it(viewList, &selected);

		if (++it != viewList.rend())
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
	if (!target || viewList.empty())
		return;

	if (thisViewIsMine(target))
	{
		if (selected)
		{
			selected->clearState(VIEW_STATE_SELECTED);
		}
		target->setState(VIEW_STATE_SELECTED);
		selected = target;
	}
}

void ViewGroup::toTheTop(View *target)
{
	if (viewList.empty())
		return;

	List<View *>::iterator it(viewList, &target);

	if (it != viewList.end())
	{
		it = viewList.erase(it);
		viewList.addHead(target);
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

bool ViewGroup::thisViewIsMine(View *who)
{
	List<View *>::iterator it(viewList, &who);

	return (it != viewList.end()) ? true : false;
}