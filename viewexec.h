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

#ifndef _VIEWEXEC_H_
#define _VIEWEXEC_H_

#include "viewgroup.h"
#include "vieweventmgr.h"

class ViewExec : public ViewGroup
{
public:
    // TBD
    void executeDialog(void){};

    // protected:
    ViewExec(Rectangle &limits, ViewRender *rnd, PaletteGroup *pals, ViewEventManager *evt, View *parent = nullptr);

    virtual void initDesktop(void);
    virtual void initMenu(void);
    virtual void run(void);

    virtual void draw(void) override;
    virtual void reDraw(void) override;
    virtual void sendEvent(Event *evt) override;

    virtual void handleEvent(Event *evt) override;

private:
    ViewEventManager *evtM;
    View *background;
};

#endif
