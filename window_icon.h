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

#ifndef _WINDOW_ICON_H_
#define _WINDOW_ICON_H_

#include "abstract_button.h"

class WindowIconClose : public AbstractButton
{
public:
	explicit WindowIconClose(Rectangle &rect);

	virtual void drawView(void) override;

protected:
	virtual void doAction(void) override;
};

class WindowIconZoom : public AbstractButton
{
public:
	explicit WindowIconZoom(Rectangle &rect);

	virtual void drawView(void) override;

protected:
	virtual void doAction(void) override;
};

#endif