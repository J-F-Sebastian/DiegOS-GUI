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

enum
{
    BUTTON_BRIGHT,
    BUTTON_DARK,
    BUTTON_MAIN,
    BUTTON_SELECTED,
    BUTTON_DISABLED,
    BUTTON_PRESSED,
    BUTTON_PAL_NUM
};

class Button : public View
{
public:
    explicit Button(Rectangle &rect);

    virtual void draw(void) override;

    virtual void handleEvent(Event *evt) override;

    bool isDown(void) { return buttonIsDown; }

protected:
    bool updateButtonState(bool eventPressed);

private:
    bool buttonIsDown;
};

#endif