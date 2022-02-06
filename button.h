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

#ifndef _BUTTON_H_
#define _BUTTON_H_

#include "view.h"

class Button : public View
{
public:
    explicit Button(Rectangle &rect);

    virtual void draw(void) override;

    virtual void handleEvent(Event *evt) override;

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

private:
    bool buttonIsDown;
};

#endif