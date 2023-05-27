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

#ifndef _FRAME_H_
#define _FRAME_H_

#include "view.h"

class Frame : public View
{
public:
    enum FrameStyle
    {
        FRAME_BEVELLED,
        FRAME_FLAT,
    };

    explicit Frame(Rectangle &rect, unsigned width = 5, enum FrameStyle style = FRAME_BEVELLED);

    virtual void draw(void) override;

    virtual void handleEvent(Event *evt) override;

    virtual bool isEventPositionValid(Event *evt) override;

private:
    enum FrameStyle style;
    unsigned width;
};

#endif