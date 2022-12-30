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

#ifndef _VIEWAPPLICATION_H_
#define _VIEWAPPLICATION_H_

#include "viewexec.h"

class ViewApplication : public ViewExec
{
public:
	ViewApplication(Rectangle &limits, ViewRender *rnd, PaletteGroup *pals, ViewEventManager *evt, View *parent = nullptr);
	virtual ~ViewApplication();

	virtual void initDesktop(void);
	virtual void initMenu(void);

	virtual void handleEvent(Event *evt) override;

private:
	View *background;
};

#endif