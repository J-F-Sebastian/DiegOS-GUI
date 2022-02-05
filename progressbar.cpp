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
#include <iostream>
#include <cstdio>

#include "progressbar.h"

ProgressBar::ProgressBar(Rectangle &rect, bool showpercent) : View(rect), showPercent(showpercent), percent(36)
{
  setOptions(VIEW_OPT_TOPSELECT);
}

void ProgressBar::draw()
{
  unsigned color;
  int pxcent;
  ViewRender *renderer = getRenderer();
  Palette *palette = getPalette();
  Rectangle viewRect;
  getExtent(viewRect);
  globalize(viewRect);

  palette->getPalette(PROGRESSBAR_BG, color);
  renderer->filledRectangle(viewRect, color);
  viewRect.zoom(-2, -2);
  pxcent = viewRect.width() * percent / 100;
  viewRect.width(pxcent);
  palette->getPalette(PROGRESSBAR_FG, color);
  renderer->filledRectangle(viewRect, color);
  if (showPercent)
  {
    char buffer[5];
    snprintf(buffer, sizeof(buffer), "%3d%%", percent);
    palette->getPalette(PROGRESSBAR_TEXT, color);
    renderer->text(viewRect, color, buffer);
  }
}

void ProgressBar::handleEvent(Event *evt)
{
  View::handleEvent(evt);

  if (isEventCmdForMe(evt))
  {
    MessageEvent *msg = evt->getMessageEvent();
    if (msg->command == CMD_UPDATE)
    {
      percent = (msg->subCommand > 100) ? 100 : msg->subCommand;
      evt->clear();
      /* Now ask for redrawing */
      sendCommand(CMD_DRAW);
    }
  }
}
