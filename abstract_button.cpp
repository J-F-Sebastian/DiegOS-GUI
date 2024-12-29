/*
 * DiegOS Operating System source code
 *
 * Copyright (C) 2012 - 2024 Diego Gallizioli
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
#include "abstract_button.h"

AbstractButton::AbstractButton(Rectangle &rect) : View(rect), buttonIsDown(false)
{
	setOptions(VIEW_OPT_SELECTABLE | VIEW_OPT_TOPSELECT);
}

void AbstractButton::handleEvent(Event *evt)
{
	View::handleEvent(evt);

	if (isEventPositionValid(evt))
	{
		// Update the pressure state, if the new state is RELEASED, it means
		// the icon was pressed and then released, in this case the object
		// will generate an event.
		// Skip move-on-hold events.
		if (!evt->testPositionalEventStatus(POS_EVT_DRAG))
		{
			bool pressed = (evt->testPositionalEventStatus(POS_EVT_PRESSED) && evt->testPositionalEventPos(POS_EVT_LEFT));
			if (updateButtonState(pressed))
			{
				setChanged(VIEW_CHANGED_REDRAW);
				/* Now ask for redrawing */
				sendCommandToTopView(CMD_REDRAW);
				if (!pressed)
					doAction();
			}
		}
	}
}

bool AbstractButton::validateCommand(const uint16_t command)
{
	switch (command)
	{
	case CMD_REL_FOCUS:
	{
		updateButtonState(false);
		setChanged(VIEW_CHANGED_REDRAW);
		/* Now ask for redrawing */
		sendCommandToTopView(CMD_REDRAW);
	}
	/* FALLTHRU */
	case CMD_DRAW:
	/* FALLTHRU */
	case CMD_REDRAW:
	/* FALLTHRU */
	case CMD_QUIT:
		return true;
	}

	return false;
}

bool AbstractButton::updateButtonState(bool eventPressed)
{
	bool toBeUpdated = (buttonIsDown != eventPressed) ? true : false;

	if (toBeUpdated)
	{
		buttonIsDown = !buttonIsDown;
	}

	return toBeUpdated;
}