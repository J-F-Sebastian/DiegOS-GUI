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

#ifndef _ABS_BUTTON_H_
#define _ABS_BUTTON_H_

#include "view.h"

class AbstractButton : public View
{
public:
	explicit AbstractButton(Rectangle &rect);

	virtual void handleEvent(Event *evt) override;

	virtual bool validateCommand(const uint16_t command) override;

	bool isDown(void) { return buttonIsDown; }

protected:
	/*
	 * Returns the update status of buttonIsDown, applying the
	 * pressure state found in eventPressed.
	 * Returned value is true if buttonIsDown value has changed.
	 *
	 * PARAMETERS IN
	 * bool eventPressed - the pressure event, up or down
	 *
	 * RETURNS
	 * true if the buttonIsDown value has changed, so the button switched
	 * from down to up, or reverse
	 * false in any other case
	 */
	bool updateButtonState(bool eventPressed);

	/*
	 * Executes an action when the status of buttonIsDown changes from
	 * true to false, which means, the user clicked on the button.
	 */
	virtual void doAction(void) = 0;

private:
	bool buttonIsDown;
};

#endif