#include "viewapplication.h"
#include "desktop.h"
#include "event_keyboard.h"

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
	evt->print();

	if (evt->isEventKey())
	{
		KeybEvent *key = evt->getKeyDownEvent();
		switch (key->keyCode)
		{
		case KBD_CODE_x:
			if (key->modifier & KBD_MOD_CTRL)
			{
				Event event;
				sendCommand(CMD_QUIT, BROADCAST_OBJECT, BROADCAST_OBJECT);
				/*
				 * On exit dequeue all pending events before termination
				 */
				while (evtM->wait(&event, 0))
					handleEvent(&event);
				draw();
				clearState(VIEW_STATE_EVLOOP);
			}
			break;
		}
	}

	ViewExec::handleEvent(evt);
}