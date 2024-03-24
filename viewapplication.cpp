/*
 * DiegOS Operating System source code
 *
 * Copyright (C) 2012 - 2022 Diego Gallizioli
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

#include "viewapplication.h"
#include "desktop.h"
#include "event_keyboard.h"

#include <iostream>

ViewApplication::ViewApplication(Rectangle &limits, ViewEventManager *evt, View *parent) : ViewExec(limits, evt, parent), background(nullptr)
{
}

ViewApplication::~ViewApplication()
{
	delete background;
}

void ViewApplication::initDesktop()
{
	Rectangle rect;
	getExtent(rect);
	background = new Desktop(rect);
	insert(background);
}

void ViewApplication::initMenu()
{
}

void ViewApplication::handleEvent(Event *evt)
{
	// evt->print();

	if (evt->isEventCommand())
	{
		MessageEvent *msg = evt->getMessageEvent();
		/*
		 * All objects in the tree
		 */
		if (isCommandAny(msg))
		{
			switch (msg->command)
			{
			case CMD_REDRAW:
				std::cout << "REDRAW CMD all" << std::endl;
				reDraw();
				evt->clear();
				break;

			case CMD_DRAW:
				std::cout << "DRAW CMD all" << std::endl;
				draw();
				evt->clear();
				break;

			case CMD_QUIT:
				std::cout << "QUIT CMD all" << std::endl;
				forEachExecuteCommand(msg);
				VIEWLISTITFOR(it)
				{
					delete (*it);
				}
				viewList.clear();
				actual = nullptr;
				evt->clear();
				break;
			}
		}
	}
	else if (evt->isEventKey())
	{
		KeybEvent *key = evt->getKeyDownEvent();
		switch (key->keyCode)
		{
		case KBD_CODE_x:
			if (key->modifier & KBD_MOD_CTRL)
			{
				Event event;
				sendCommand(CMD_QUIT);
				/*
				 * On exit dequeue all pending events before termination
				 */
				while (evtM->wait(&event, 100))
					handleEvent(&event);
				draw();
				clearState(VIEW_STATE_EVLOOP);
			}
			break;
		}
	}

	ViewExec::handleEvent(evt);
}