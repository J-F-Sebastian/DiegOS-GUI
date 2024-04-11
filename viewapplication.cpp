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
	static unsigned i = 0;

	// evt->print();
	++i;

	if (evt->isEventKey())
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
				// draw();
				clearState(VIEW_STATE_EVLOOP);
			}
			break;
		}
	}

	ViewExec::handleEvent(evt);
}