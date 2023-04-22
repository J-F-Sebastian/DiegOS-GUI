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

#include "viewgroup.h"

#define VIEWLISTITFOR(x) for (List<View *>::iterator x = viewList.begin(); x != viewList.end(); x++)
#define VIEWLISTREVITFOR(x) for (List<View *>::riterator x = viewList.rbegin(); x != viewList.rend(); x++)

ViewGroup::ViewGroup(Rectangle &limits, View *parent) : View(limits, parent), lastLimits(limits), actual(nullptr), viewList()
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

	if (getResizeMode(VIEW_ZOOMED))
		return false;

	sizeLimits(min, max);
	if ((loc.width() > max.x) ||
	    (loc.width() < min.x) ||
	    (loc.height() > max.y) ||
	    (loc.height() < min.y))
		return false;

	getBorders(update);
	delta.x = loc.width() - update.width();
	delta.y = loc.height() - update.height();
	setBorders(loc);

	VIEWLISTITFOR(it)
	{
		(*it)->calcLimits(delta, update);
		(*it)->setLocation(update);
	}

	return true;
}

void ViewGroup::draw()
{
	if (getState(VIEW_STATE_EXPOSED))
	{
		//  Draw children back-to-top, following the painter algorithm
		VIEWLISTREVITFOR(it)
		{
			if ((*it)->getState(VIEW_STATE_EXPOSED))
				(*it)->draw();
		}
	}
}

void ViewGroup::reDraw()
{
	if (getState(VIEW_CHANGED_REDRAW))
	{
		// Draw children back-to-top, following the painter algorithm
		VIEWLISTREVITFOR(it)
		{
			(*it)->reDraw();
		}
		clearChanged(VIEW_CHANGED_REDRAW);
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
		VIEWLISTITFOR(it)
		{
			(*it)->handleEvent(evt);
			if (evt->isEventUnknown())
				return;
		}
		evt->clear();
	}
	/*
	 * If the event is a key press, process the event only if we are selected and focused.
	 */
	else if (evt->isEventKey())
	{
		if (getStateAll(VIEW_STATE_SELECTED | VIEW_STATE_FOCUSED))
		{
			KeyDownEvent *key = evt->getKeyDownEvent();
			if (key->modifier == KBD_MOD_NONE)
			{
				switch (key->keyCode)
				{
				case KBD_CODE_F5:
					if (getResizeMode(VIEW_RESIZEABLE))
					{
						if (getResizeMode(VIEW_ZOOMED))
							restore();
						else
							maximize();
						evt->clear();
					}
					break;
				}
			}
		}
		if (actual && !evt->isEventUnknown())
			actual->handleEvent(evt);
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
	 * |               |          |
	 * |  child view   |  forward |
	 * |               |          |
	 * +---------------+----------+
	 * |               |          |
	 * |  unknown view |  clear   |
	 * |               |          |
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
	else if (evt->isEventCommand())
	{
		MessageEvent *msg = evt->getMessageEvent();
		if (isEventCmdForMe(evt))
		{
			switch (msg->command)
			{
			case CMD_QUIT:
				std::cout << "QUIT CMD " << reinterpret_cast<intptr_t>(msg->targetObject) << std::endl;
				/*
				 * We are instructed to close all of our views, in fact the target is ignored.
				 */
				VIEWLISTITFOR(it)
				{
					delete (*it);
				}
				viewList.clear();
				break;

			case CMD_FOREGROUND:
				std::cout << "Foreground CMD " << reinterpret_cast<intptr_t>(msg->targetObject) << std::endl;
				if (msg->targetObject == this)
					setForeground();
				break;

			case CMD_SELECT:
				std::cout << "Select CMD " << reinterpret_cast<intptr_t>(msg->targetObject) << std::endl;
				selectView(reinterpret_cast<View *>(msg->targetObject));
				break;

			case CMD_CLOSE:
				std::cout << "CLOSE CMD " << reinterpret_cast<intptr_t>(msg->targetObject) << std::endl;
				/*
				 * We are instructed to close, so send an event to our owner
				 */
				if (msg->targetObject == this)
				{
					sendCommand(CMD_CLOSE, getParent(), this);
				}
				/*
				 * If the target is BROADCAST_OBJECT close all views
				 */
				else if (msg->targetObject == BROADCAST_OBJECT)
				{
					List<View *>::iterator it(viewList);
					while (it != viewList.end())
					{
						if ((*it)->getState(VIEW_STATE_FOCUSED))
							focusNext(true);
						if ((*it)->getState(VIEW_STATE_SELECTED))
							selectNext(true);
						delete (*it);
						it = viewList.erase(it);
					}
				}
				/*
				 * Check if we are instructed to close one of our views
				 */
				else
				{
					View *target = reinterpret_cast<View *>(msg->targetObject);
					if (target)
					{
						List<View *>::iterator it(viewList, &target);
						if (it != viewList.end())
						{
							if (remove(target))
							{
								delete target;
								/* Now ask for redrawing */
								sendCommand(CMD_DRAW);
							}
						}
					}
				}
				break;

			case CMD_MAXIMIZE:
				if (msg->targetObject == this)
					maximize();
				break;

			case CMD_RESTORE:
				if (msg->targetObject == this)
					restore();
				break;

			case CMD_DRAW:
			{
				if (isEventCmdTargetMe(evt))
				{
					draw();
				}
				else
				{
					View *target = static_cast<View *>(msg->targetObject);
					List<View *>::iterator it(viewList, &target);
					if (it != viewList.end())
						(*it)->draw();
				}
			}
			break;

			case CMD_REDRAW:
				reDraw();
				break;
			}

			if (msg->destObject == this)
			{
				evt->clear();
			}
			/*
			 * Forward message to all children.
			 */
			else
			{
				VIEWLISTITFOR(it)
				{
					(*it)->handleEvent(evt);
				}
			}
		}
		else
		{
			/*
			 * Forward message, if the destination is a child go unicast.
			 * If the destination is unknown, go broadcast.
			 */
			View *target = reinterpret_cast<View *>(msg->targetObject);
			List<View *>::iterator lkup(viewList, &target);

			if (lkup != viewList.end())
			{
				(*lkup)->handleEvent(evt);
			}
			else
			{
				VIEWLISTITFOR(it)
				{
					(*it)->handleEvent(evt);
				}
			}
		}
	}
	else if (evt->isEventUnknown())
	{
		std::cout << "UNKNOWN EVENT: ";
		evt->print();
	}
}

bool ViewGroup::executeCommand(const uint16_t command)
{
	switch (command)
	{
	case CMD_DRAW:
		draw();
		return true;

	case CMD_REDRAW:
		reDraw();
		return true;

	case CMD_REQ_FOCUS:
		if (validateCommand(command))
		{
			/*
			 * If we are the root node or we are running the
			 * event loop (could it be different ?!?) return true
			 * as we cannot grab or release the focus.
			 */
			if (!getParent() || getState(VIEW_STATE_EVLOOP))
				return true;

			/*
			 * A cast resulting in nullptr would mean a View was set as
			 * a parent of a group .... does it make sense ?
			 */
			ViewGroup *parent = dynamic_cast<ViewGroup *>(getParent());
			if (parent)
			{
				if (this != parent->actualView())
				{
					if (parent->actualView()->executeCommand(CMD_REL_FOCUS))
					{
						return parent->focusView(this);
					}
				}
				return true;
			}
		}
		break;

	case CMD_REL_FOCUS:
		if (validateCommand(command))
		{
			/*
			 * If we are the root node or we are running the
			 * event loop (could it be different ?!?) return true
			 * as we cannot grab or release the focus.
			 */
			if (!getParent() || getState(VIEW_STATE_EVLOOP))
				return true;

			bool retval = true;

			if (actual)
				retval = actual->executeCommand(command);

			if (retval)
			{
				clearState(VIEW_STATE_FOCUSED | VIEW_STATE_SELECTED);
				actual = nullptr;
				return true;
			}
		}
		break;
	}

	return false;
}

bool ViewGroup::validateCommand(const uint16_t command)
{
	return true;
}

void ViewGroup::setForeground()
{
	setState(VIEW_STATE_FOREGROUND);
	VIEWLISTITFOR(it)
	{
		(*it)->setForeground();
	}
}

void ViewGroup::setBackground()
{
	clearState(VIEW_STATE_FOREGROUND);
	VIEWLISTITFOR(it)
	{
		(*it)->setBackground();
	}
}

void ViewGroup::insert(View *newView)
{
	if (newView)
	{
		newView->setParent(this);
		if (newView->getOptions(VIEW_OPT_TOPSELECT))
		{
			viewList.addHead(newView);
			if (actual)
			{
				actual->setBackground();
			}
			if (focusView(newView))
			{
				newView->setForeground();
			}
			else
			{
				actual->setForeground();
			}
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
		}
	}
}

bool ViewGroup::remove(View *target)
{
	if (!target)
		return false;

	if (target == actual)
	{
		if (!focusNext(true))
		{
			actual = nullptr;
		}
	}
	List<View *>::iterator it(viewList, &target);
	if (it != viewList.end())
	{
		it = viewList.erase(it);
		return true;
	}

	return false;
}

View *ViewGroup::actualView()
{
	return actual;
}

bool ViewGroup::focusNext(bool forward)
{
	View *temp = nullptr;

	if (viewList.count() < 1)
		return false;

	// FIXME missing check if focus can be handed off
	if (forward)
	{
		List<View *>::iterator it(viewList, &actual);

		if (++it != viewList.end())
			temp = (*it);
		else
			temp = viewList.getHead();
	}
	else
	{
		List<View *>::riterator it(viewList, &actual);

		if (++it != viewList.rend())
			temp = (*it);
		else
			temp = viewList.getTail();
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
		List<View *>::iterator it(viewList, &actual);

		if (++it != viewList.end())
			temp = (*it);
		else
			temp = viewList.getHead();
	}
	else
	{
		List<View *>::riterator it(viewList, &actual);

		if (++it != viewList.rend())
			temp = (*it);
		else
			temp = viewList.getTail();
	}

	selectView(temp);
}

bool ViewGroup::focusView(View *target)
{
	if (!target)
		return false;

	if (target->focus())
	{
		selectView(target);
		return true;
	}

	return false;
}

void ViewGroup::selectView(View *target)
{
	if (!target || viewList.empty())
		return;

	if (thisViewIsMine(target))
	{
		if (actual)
		{
			actual->clearState(VIEW_STATE_SELECTED | VIEW_STATE_FOCUSED);
		}
		actual = target;
		if (actual->getOptions(VIEW_OPT_TOPSELECT))
			toTheTop(actual);
	}
}

void ViewGroup::toTheTop(View *target)
{
	if (viewList.empty())
		return;

	List<View *>::iterator it(viewList, &target);

	if (it != viewList.end())
	{
		viewList.getHead()->setBackground();
		it = viewList.erase(it);
		viewList.addHead(target);
		viewList.getHead()->setForeground();
		/* Now ask for redrawing */
		sendCommand(CMD_DRAW);
	}
}

void ViewGroup::maximize()
{
	if (getParent())
	{
		getBorders(lastLimits);
		Rectangle max;
		getParent()->getExtent(max);
		setLocation(max);
		lastrflags = getResizeMode();
		setResizeMode(VIEW_ZOOMED);
		/* Now ask for redrawing */
		sendCommand(CMD_DRAW, getParent(), getParent());
	}
}

void ViewGroup::minimize()
{
	// FIX ME
	clearResizeMode(VIEW_ZOOMED);
	setResizeMode(lastrflags);
}

void ViewGroup::restore()
{
	clearResizeMode(VIEW_ZOOMED);
	setResizeMode(lastrflags);
	setLocation(lastLimits);
	/* Now ask for redrawing */
	sendCommand(CMD_DRAW);
}

bool ViewGroup::thisViewIsMine(View *who)
{
	List<View *>::iterator it(viewList, &who);

	return (it != viewList.end()) ? true : false;
}

void ViewGroup::setExposed(bool exposed)
{
	View::setExposed(exposed);

	VIEWLISTITFOR(it)
	{
		(*it)->setExposed(exposed);
	}
}

void ViewGroup::clearExposed(Rectangle &covered)
{
	Rectangle local;

	View::clearExposed(covered);
	getBorders(local);

	if (local.intersect(covered))
	{
		local.intersection(covered);
		localize(local);
		VIEWLISTITFOR(it)
		{
			(*it)->clearExposed(local);
		}
	}
}

void ViewGroup::computeExposure()
{
	/*
	 * Reset exposure of all views; note that invisible views
	 * will be forced to be not exposed.
	 */
	setExposed(true);

	/*
	 * Make a list of visible views to compute the
	 * covering areas
	 */
	List<View *> visible;

	VIEWLISTITFOR(it)
	{
		if ((*it)->getState(VIEW_STATE_VISIBLE))
			visible.addTail(*it);
	}

	/*
	 * Optimization (?!?): if no view is visible, or there is only one
	 * visible view, drop out
	 */
	if (visible.count() < 2)
		return;

	List<View *>::iterator target(visible);
	List<View *>::iterator firstVisible = target;

	while (++target != visible.end())
	{
		List<View *>::iterator predecessors = visible.begin();
		while (predecessors != target)
		{
			Rectangle predR;
			(*predecessors)->getBorders(predR);
			(*target)->clearExposed(predR);
			predecessors++;
		}
	}
}