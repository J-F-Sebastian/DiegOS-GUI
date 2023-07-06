#include "viewapplication.h"
#include "desktop.h"
#include "event_keyboard.h"

#define VIEWLISTITFOR(x) for (List<View *>::iterator x = viewList.begin(); x != viewList.end(); x++)

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