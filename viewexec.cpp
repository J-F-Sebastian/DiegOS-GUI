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

#include "viewinstances.h"
#include "viewexec.h"
#include "background.h"
#include "event_keyboard.h"

#include <iostream>

ViewExec::ViewExec(Rectangle &limits, ViewEventManager *evt, View *parent) : ViewGroup(limits, 0, parent), evtM(evt)
{
	clearOptions(VIEW_OPT_ALL);
	setState(VIEW_STATE_SELECTED | VIEW_STATE_EVLOOP | VIEW_STATE_FOCUSED);
}

void ViewExec::run()
{
	Event event;

	reDraw();

	while (getState(VIEW_STATE_EVLOOP))
	{
		while (evtM->wait(&event, 1000))
		{
			handleEvent(&event);
			if (!event.isEventUnknown())
				std::cout << " ERROR !!!" << std::endl;
		}
	}
}

void ViewExec::draw()
{
	if (getState(VIEW_STATE_EVLOOP))
	{
		GZBuffer->clear();
		computeExposure();
		GRenderer->start();
		GRenderer->clear(0);
		ViewGroup::draw();
		GRenderer->show();
	}
}

void ViewExec::reDraw()
{
	if (getState(VIEW_STATE_EVLOOP))
	{
		GZBuffer->clear();
		computeExposure();
		GRenderer->start();
		GRenderer->clear(0);
		ViewGroup::reDraw();
		ViewGroup::draw();
		GRenderer->show();
	}
}

void ViewExec::sendEvent(Event *evt)
{
	if (getState(VIEW_STATE_EVLOOP))
	{
		evtM->put(evt);
	}
	else
		View::sendEvent(evt);
}

void ViewExec::handleEvent(Event *evt)
{
	if (evt->isEventKey())
	{
		KeybEvent *key = evt->getKeyDownEvent();
		switch (key->keyCode)
		{
		case KBD_CODE_F4:
			if (key->modifier & KBD_MOD_ALT)
			{
				// ViewGroup will take care of this
				sendCommand(CMD_CLOSE, this, actual);
				evt->clear();
			}
			break;
		}
	}

	ViewGroup::handleEvent(evt);
}