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
#include "viewzbuffer.h"

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
	if (getState(VIEW_STATE_EXPOSED) && getChanged(VIEW_CHANGED_REDRAW))
	{
		// Draw children back-to-top, following the painter algorithm
		VIEWLISTREVITFOR(it)
		{
			if ((*it)->getState(VIEW_STATE_EXPOSED))
				(*it)->reDraw();
		}
		clearChanged(VIEW_CHANGED_REDRAW);
	}
}

void ViewGroup::handleEvent(Event *evt)
{
	/* If the event is positional, look for a view including the event in its
	 * boundaries; first view accepting the coordinates of the event will process the
	 * event with handleEvent() method.
	 */
	if (isEventPositionValid(evt))
	{
		VIEWLISTITFOR(it)
		{
			if ((*it)->isEventPositionInRange(evt))
			{
				// std::cout << "GOTCHA! " << std::hex << (intptr_t)(*it) << std::dec << std::endl;
				(*it)->handleEvent(evt);
				break;
			}
		}
	}
	/*
	 * If the event is a key press, process the event only if we are selected and focused.
	 */
	else if (evt->isEventKey())
	{
		if (getStateAll(VIEW_STATE_SELECTED | VIEW_STATE_FOCUSED))
		{
			KeybEvent *key = evt->getKeyDownEvent();
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
			if (actual && !evt->isEventUnknown())
				actual->handleEvent(evt);
		}
	}
	/*
	 * If the event is a message (a command) then the event is processed if the destination
	 * is either this view or the broadcast object.
	 * The event will be propagated to children if the destination is the broadcast object
	 * or is NOT this view.
	 *
	 * +---------------+--------------+
	 * |  destination  |    action    |
	 * +---------------+--------------+
	 * |               |    execute   |
	 * |  this view    |      and     |
	 * |               |     clear    |
	 * +---------------+--------------+
	 * |               |    execute   |
	 * |  broadcast    |      and     |
	 * |               |    forward   |
	 * |               |      to      |
	 * |               |    children  |
	 * +---------------+--------------+
	 * |               |    forward   |
	 * |  child view   |      to      |
	 * |               |     child    |
	 * +---------------+--------------+
	 * |               |    forward   |
	 * |  unknown view |      to      |
	 * |               |    children  |
	 * +---------------+--------------+
	 *
	 * A broadcast destination will make a message be processed by all handleEvent methods
	 * of all Views in the system.
	 * A unicast destination will make a message be processed by handleEvent methods
	 * until the destination view is reached; the message will be forwarded from root to leaves
	 * until the destination leave (a View) is reached.
	 *
	 * The targetObject is part of the header message but is used to address the action
	 * of the command, not the destination.
	 * Due to this a valid target object can be either this view, a child view or the broadcast address.
	 * Other values must be ignored.
	 *
	 * +---------------+--------------+
	 * |    target     |    action    |
	 * +---------------+--------------+
	 * |               |    execute   |
	 * |  this view    |      and     |
	 * |               |     clear    |
	 * +---------------+--------------+
	 * |               |    execute   |
	 * |  broadcast    |      for     |
	 * |               |      all     |
	 * |               |    children  |
	 * +---------------+--------------+
	 * |               |    execute   |
	 * |  child view   |      for     |
	 * |               |     child    |
	 * |               |      and     |
	 * |               |     clear    |
	 * +---------------+--------------+
	 * |               |              |
	 * |  unknown view |     skip     |
	 * |               |              |
	 * +---------------+--------------+
	 */
	else if (evt->isEventCommand())
	{
		MessageEvent *msg = evt->getMessageEvent();
		/*
		 * This object as destination and target
		 */
		if (isCommandMe(msg))
		{
			switch (msg->command)
			{
			case CMD_FOREGROUND:
				std::cout << "Foreground CMD " << reinterpret_cast<intptr_t>(msg->targetObject) << std::endl;
				setForeground();
				break;

			case CMD_CLOSE:
				std::cout << "CLOSE CMD " << reinterpret_cast<intptr_t>(msg->targetObject) << std::endl;
				/*
				 * We are instructed to close, so send an event to our owner
				 */
				sendCommand(CMD_CLOSE, getParent(), this);
				break;

			case CMD_MAXIMIZE:
				std::cout << "MAXIMIZE CMD " << reinterpret_cast<intptr_t>(msg->targetObject) << std::endl;
				maximize();
				break;

			case CMD_RESTORE:
				std::cout << "RESTORE CMD " << reinterpret_cast<intptr_t>(msg->targetObject) << std::endl;
				restore();
				break;
			}
			/*
			 * Cleanup anyway, the message was for THIS object.
			 */
			evt->clear();
		}
		/*
		 * All objects as a destination
		 */
		else if (isCommandForAll(msg))
		{
			/*
			 * Forward message to all children.
			 */
			VIEWLISTITFOR(it)
			{
				(*it)->handleEvent(evt);
			}
			switch (msg->command)
			{
			case CMD_QUIT:
				std::cout << "QUIT CMD at " << reinterpret_cast<intptr_t>(this) << " for " << reinterpret_cast<intptr_t>(msg->targetObject) << std::endl;
				/*
				 * We are instructed to close so we do close all of our views, in fact the target is ignored.
				 * NOTE: CMD_QUIT is nonsense with a specific destination ...
				 */
				VIEWLISTITFOR(it)
				{
					delete (*it);
				}
				viewList.clear();
				actual = nullptr;
				break;
			}
			/*
			 * No event cleanup !!!
			 */
		}
		/*
		 * This object as a destination
		 */
		else if (isCommandForMe(msg))
		{
			switch (msg->command)
			{
			case CMD_SELECT:
				std::cout << "Select CMD for " << reinterpret_cast<intptr_t>(msg->targetObject) << std::endl;
				selectView(reinterpret_cast<View *>(msg->targetObject));
				break;

			case CMD_CLOSE:
				std::cout << "CLOSE CMD for " << reinterpret_cast<intptr_t>(msg->targetObject) << std::endl;
				/*
				 * If the target is BROADCAST_OBJECT close all views
				 */
				if (isCommandTargetAll(msg))
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
							}
						}
					}
				}
				/* Now ask for redrawing */
				sendCommand(CMD_DRAW);
				break;

			case CMD_DRAW:
			{
				View *target = static_cast<View *>(msg->targetObject);
				if (thisViewIsMine(target))
					target->draw();
			}
			break;
			}
			/*
			 * Cleanup anyway, the message was for THIS object.
			 */
			evt->clear();
		}
		/*
		 * Destination unknown, forward message to all children.
		 */
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

	case CMD_MAXIMIZE:
		maximize();
		return true;

	case CMD_RESTORE:
		restore();
		return true;

	case CMD_CLOSE:
		sendCommand(CMD_CLOSE, getParent(), this);
		return true;

	case CMD_FOREGROUND:
		setForeground();
		break;

	case CMD_QUIT:
		VIEWLISTITFOR(it)
		{
			delete (*it);
		}
		viewList.clear();
		actual = nullptr;
		break;
	}

	return false;
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
			focusView(newView);
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
		// sendCommand(CMD_DRAW);
	}
}

void ViewGroup::maximize()
{
	if (getResizeModeAll(VIEW_BOUNDED))
		return;

	if (getParent())
	{
		getBorders(lastLimits);
		Rectangle max;
		getParent()->getExtent(max);
		setLocation(max);
		lastrflags = getResizeMode();
		setResizeMode(VIEW_ZOOMED);
		/* Now ask for redrawing */
		sendCommand(CMD_DRAW);
	}
}

void ViewGroup::minimize()
{
	if (getResizeModeAll(VIEW_BOUNDED))
		return;
	// FIX ME
	clearResizeMode(VIEW_ZOOMED);
	setResizeMode(lastrflags);
	/* Now ask for redrawing */
	sendCommand(CMD_DRAW);
}

void ViewGroup::restore()
{
	if (getResizeModeAll(VIEW_BOUNDED))
		return;

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
	exposed = false;

	VIEWLISTITFOR(it)
	{
		if ((*it)->getState(VIEW_STATE_EXPOSED))
			exposed = true;
	}

	View::setExposed(exposed);
}

void ViewGroup::computeExposure()
{
	/*
	 * Set in the Z buffer the layer depth of each view
	 */
	VIEWLISTITFOR(it)
	{
		(*it)->computeExposure();
	}

	/*
	 * true or false does not matter, the value is
	 * recomputed by the method.
	 */
	setExposed(true);
}