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
#include "viewinstances.h"

ViewGroup::ViewGroup(Rectangle &limits, View *parent) : View(limits, parent), lastLimits(limits), actual(nullptr), listHead(nullptr), listSize(0), lastrflags(0)
{
	setOptions(VIEW_OPT_SELECTABLE);
	updateRenderBuffer();
}

ViewGroup::~ViewGroup()
{
	while (listHead)
	{
		View *next = listHead->getNext();
		delete listHead;
		listHead = next;
	}

	actual = nullptr;
	listSize = 0;
}

bool ViewGroup::setLocation(const Rectangle &loc)
{
	Rectangle update;
	Point delta(0, 0);
	Point min, max;

	if (getResizeMode(VIEW_ZOOMED))
		return false;

	getBorders(update);

	if (update == loc)
		return true;

	sizeLimits(min, max);
	if ((loc.width() > max.x) ||
	    (loc.width() < min.x) ||
	    (loc.height() > max.y) ||
	    (loc.height() < min.y))
		return false;

	delta.x = loc.width() - update.width();
	delta.y = loc.height() - update.height();
	setBorders(loc);

	forEachView([&delta, &update](View *head)
		    {
			head->calcLimits(delta, update);
			head->setLocation(update); });

	return true;
}

void ViewGroup::draw()
{
	if (getState(VIEW_STATE_EXPOSED))
	{
		//  Draw children back-to-top, following the painter algorithm
		if (listSize)
		{
			View **storage = new View *[listSize];
			unsigned i = 0;

			/*
			 * Accumulate the exposed views
			 */
			forEachView([&i, storage](View *head)
				    {
					if (head->getState(VIEW_STATE_EXPOSED))
						storage[i++] = head; });

			/*
			 * Draw the accumulated views in reverse order.
			 * Owner buffer is automatically used by views.
			 */
			while (i--)
			{
				storage[i]->draw();
			}

			delete[] storage;
		}
	}
}

void ViewGroup::reDraw()
{
	if (getChanged(VIEW_CHANGED_REDRAW))
	{
		// Update buffers
		if (listSize)
		{
			/*
			 * Update exposed buffers
			 */
			forEachView([](View *head)
				    {
					if (head->getState(VIEW_STATE_EXPOSED))
						head->reDraw(); });
		}
		clearChanged(VIEW_CHANGED_REDRAW);
	}
}

void ViewGroup::drawView()
{
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
		View *toHandle = forEachViewUntilTrue([evt](View *head) -> bool
						      { return head->isEventPositionInRange(evt); });

		if (toHandle)
			toHandle->handleEvent(evt);
		/*
		 * Cleanup anyway, the message was for THIS object.
		 */
		evt->clear();
	}
	/*
	 * If the event is a key press, process the event only if we are focused.
	 */
	else if (evt->isEventKey())
	{
		if (getState(VIEW_STATE_FOCUSED))
		{
			KeybEvent *key = evt->getKeyDownEvent();
			if ((key->modifier & (KBD_MOD_ALT | KBD_MOD_CTRL | KBD_MOD_SHIFT)) == 0)
			{
				switch (key->keyCode)
				{
				case KBD_CODE_F5:
					if (executeCommand(CMD_ZOOM))
						evt->clear();
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
	 * |               |    forward   |
	 * |  broadcast    |      to      |
	 * |               |    children  |
	 * |               |      and     |
	 * |               |     clear    |
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
	 * The message MUST have this view or the broadcast object as destination .
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
		 * This object as destination
		 */
		if (isCommandForMe(msg))
		{
			View *caller = reinterpret_cast<View *>(msg->senderObject);
			View *target = reinterpret_cast<View *>(msg->targetObject);

			// evt->print();
			switch (msg->command)
			{
			case CMD_CLOSE:
				/*
				 * Check if we are instructed to close one of our views
				 */
				if (thisViewIsMine(target))
				{
					if (remove(target))
					{
						delete target;
						/* Now ask for redrawing */
						sendCommand(CMD_DRAW);
					}
					evt->clear();
				}
				else if (target == BROADCAST_OBJECT)
				{
					/*
					 * If the target is BROADCAST_OBJECT close all views
					 */
					while (listHead)
					{
						View *next = listHead->getNext();
						if (listHead->getState(VIEW_STATE_FOCUSED))
							focusNext(true);
						else if (listHead->getState(VIEW_STATE_SELECTED))
							selectNext(true);
						delete listHead;
						listHead = next;
					}
					actual = nullptr;
					listSize = 0;
					evt->clear();
				}
				break;
			}

			if (!evt->isEventUnknown())
			{
				if (executeCommand(msg->command, caller))
					evt->clear();
			}
			/*
			 * Unsupported messages will generate a trace in the main loop!
			 */
		}
		/*
		 * All objects as a destination
		 */
		else if (isCommandForAll(msg))
		{
			executeCommand(msg->command, reinterpret_cast<View *>(msg->targetObject));

			if (!evt->isEventUnknown())
			{
				/*
				 * Forward message to all children.
				 */
				forEachView([evt](View *head)
					    { head->handleEvent(evt); });

				/*
				 * No event cleanup unless we are the main loop
				 */
				if (getState(VIEW_STATE_EVLOOP))
					evt->clear();
			}
		}
		/*
		 * Destination unknown, forward message to all children.
		 */
		else
		{
			View *target = reinterpret_cast<View *>(msg->targetObject);
			if (target)
			{
				forEachViewUntilTrue([target, evt](View *head) -> bool
						     {
							target->handleEvent(evt);
							return (target == head) ? true : false; });
			}
		}
	}
}

bool ViewGroup::executeCommand(const uint16_t command, View *caller)
{
	if (validateCommand(command))
	{
		switch (command)
		{
		case CMD_DRAW:
			if (thisViewIsMine(caller))
				caller->draw();
			else
				draw();
			return true;

		case CMD_REDRAW:
			if (thisViewIsMine(caller))
				caller->reDraw();
			else
				reDraw();
			return true;

		case CMD_REQ_FOCUS:
		{
			/*
			 * The focused object has the focus !!!
			 */
			if (caller && (actual == caller))
			{
				return true;
			}

			bool retval = true;

			if (getState(VIEW_STATE_FOCUSED))
			{
				if (actual)
				{
					/*
					 * Try to make the focused objects release the focus.
					 * If this operation is denied, return false.
					 */
					retval = actual->executeCommand(CMD_REL_FOCUS);
				}
			}
			else if (getParent())
			{
				retval = getParent()->executeCommand(CMD_REQ_FOCUS, this);
			}

			/*
			 * The focused view released the focus OR there is no focused view
			 * to ask.
			 */
			if (retval && caller)
			{
				actual = caller;
			}

			return retval;
		}
		break;

		case CMD_REL_FOCUS:
		{
			bool retval = true;

			if (getState(VIEW_STATE_FOCUSED))
			{
				if (actual)
				{
					/*
					 * Try to make the focused objects release the focus.
					 * If this operation is denied, return false.
					 */
					retval = actual->executeCommand(CMD_REL_FOCUS);
				}
			}

			/*
			 * The focused view released the focus OR there is no focused view
			 * to ask.
			 */
			if (retval)
			{
				clearState(VIEW_STATE_FOCUSED | VIEW_STATE_SELECTED);
				actual = nullptr;
			}

			return retval;
		}
		break;

		case CMD_SELECT:
			return selectView(caller);

		case CMD_MAXIMIZE:
			maximize();
			return true;

		case CMD_RESTORE:
			restore();
			return true;

		case CMD_ZOOM:
			if (getResizeMode(VIEW_RESIZEABLE))
			{
				if (getResizeMode(VIEW_ZOOMED))
					restore();
				else
					maximize();

				return true;
			}
			break;

		case CMD_CLOSE:
			if (thisViewIsMine(caller))
			{
				/*
				 * We need to close, ask parent to do it
				 */
				sendCommandToParent(CMD_CLOSE);
				return true;
			}
			break;

		case CMD_FOREGROUND:
			setForeground();
			return true;

		case CMD_QUIT:
			while (listHead)
			{
				View *next = listHead->getNext();
				delete listHead;
				listHead = next;
			}
			actual = nullptr;
			listSize = 0;
			return true;
		}
	}
	return false;
}

bool ViewGroup::validateCommand(const uint16_t command)
{
	switch (command)
	{
	case CMD_DRAW:
	/* FALLTHRU */
	case CMD_REDRAW:
	/* FALLTHRU */
	case CMD_REL_FOCUS:
	/* FALLTHRU */
	case CMD_REQ_FOCUS:
	/* FALLTHRU */
	case CMD_SELECT:
	/* FALLTHRU */
	case CMD_MAXIMIZE:
	/* FALLTHRU */
	case CMD_RESTORE:
	/* FALLTHRU */
	case CMD_ZOOM:
	/* FALLTHRU */
	case CMD_CLOSE:
	/* FALLTHRU */
	case CMD_FOREGROUND:
	/* FALLTHRU */
	case CMD_QUIT:
		return true;
	}

	return false;
}

void ViewGroup::forEachExecuteCommand(MessageEvent *cmd)
{
	forEachView([cmd](View *head)
		    { head->executeCommand(cmd->command); });
}

void ViewGroup::setForeground()
{
	View::setForeground();
	forEachView([](View *head)
		    { head->setForeground(); });
}

void ViewGroup::setBackground()
{
	View::setBackground();
	forEachView([](View *head)
		    { head->setBackground(); });
}

void ViewGroup::insert(View *newView)
{
	if (newView)
	{
		newView->setParent(this);
		/*
		 * Insert to the front
		 */
		if (listHead)
		{
			newView->setNext(listHead);
		}
		else
		{
			newView->setNext(nullptr);
		}
		listHead = newView;
		listSize++;
	}
}

bool ViewGroup::remove(View *target)
{
	if (!target || !listSize)
		return false;

	if (target == actual)
	{
		/*
		 * We are removing the view, so if this view has the focus,
		 * and it is not possible to focus another view, the actual
		 * pointer must be null.
		 */
		if (!focusNext(true))
		{
			actual = nullptr;
		}
	}

	View *head = listHead;
	if (head == target)
	{
		listHead = listHead->getNext();
		listSize--;
		return true;
	}

	while (head->getNext())
	{
		if (head->getNext() == target)
		{
			head->setNext(target->getNext());
			listSize--;
			return true;
		}
		head = head->getNext();
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

	if (listSize < 1)
		return false;

	if (actual == nullptr)
	{
		temp = listHead;
	}
	else
	{
		if (forward)
		{
			temp = actual->getNext();
			if (temp == nullptr)
				temp = listHead;
		}
		else
		{
		}
	}

	return focusView(temp);
}

void ViewGroup::selectNext(bool forward)
{
	View *temp = nullptr;

	if (listSize < 1)
		return;

	if (actual == nullptr)
	{
		temp = listHead;
	}
	else
	{
		if (forward)
		{
			temp = actual->getNext();
			if (temp == nullptr)
				temp = listHead;
		}
		else
		{
		}
	}

	selectView(temp);
}

bool ViewGroup::focusView(View *target)
{
	std::cout << std::hex << "PTR " << target << std::endl;
	if (!target)
		return false;

	if (target->focus())
	{
		selectView(target);
		return true;
	}

	return false;
}

bool ViewGroup::selectView(View *target)
{
	if (!target || !listSize)
		return false;

	if (thisViewIsMine(target))
	{
		actual = target;
		if (actual->getOptions(VIEW_OPT_TOPSELECT))
			toForeground(actual);
		return true;
	}

	return false;
}

void ViewGroup::toForeground(View *target)
{
	if (listHead != target)
	{
		/*
		 * Update Foreground/Background status
		 */
		listHead->setBackground();
		/*
		 * temp points to target's father
		 */
		View *temp = forEachViewUntilTrue([target](View *v) -> bool
						  { return (v->getNext() == target) ? true : false; });

		/*
		 * Unlink target setting its father "next" points to target's "next".
		 */
		temp->setNext(target->getNext());
		/*
		 * Promote target to the top of the list
		 */
		target->setNext(listHead);
		listHead = target;
	}

	target->setForeground();

	/* Now ask for redrawing */
	sendCommand(CMD_REDRAW);
}

void ViewGroup::toBackground(View *target)
{
	/*
	 * Update Foreground/Background status
	 */
	target->setBackground();

	/* Now ask for redrawing */
	sendCommand(CMD_REDRAW);
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
		sendCommand(CMD_REDRAW);
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
	sendCommand(CMD_REDRAW);
}

void ViewGroup::restore()
{
	if (getResizeModeAll(VIEW_BOUNDED))
		return;

	clearResizeMode(VIEW_ZOOMED);
	setResizeMode(lastrflags);
	setLocation(lastLimits);
	/* Now ask for redrawing */
	sendCommand(CMD_REDRAW);
}

bool ViewGroup::thisViewIsMine(View *who)
{
	return forEachViewUntilTrue([who](View *head) -> bool
				    { return (head == who) ? true : false; });
}

void ViewGroup::setExposed(bool exposed)
{
	exposed = forEachViewUntilTrue([](View *head) -> bool
				       { return (head->getState(VIEW_STATE_EXPOSED)) ? true : false; });

	View::setExposed(exposed);
}

void ViewGroup::computeExposure()
{
	/*
	 * Set in the Z buffer the layer depth of each view
	 */
	forEachView([](View *head)
		    { head->computeExposure(); });

	/*
	 * true or false does not matter, the value is
	 * recomputed by the method.
	 */
	setExposed(true);
}