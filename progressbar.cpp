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

#include "progressbar.h"

ProgressBar::ProgressBar(Rectangle &rect, bool showpercent) : View(rect), showPercent(showpercent)
{
  setPercent(36);
}

void ProgressBar::setPercent(unsigned value)
{
  if (value > 100)
    value = 100;
  percent = value;
}

void ProgressBar::draw()
{
  unsigned color;
  int pxcent, pxcent2;

  Rectangle viewRect;
  getExtent(viewRect);
  globalize(viewRect);
  Rectangle temp(viewRect);
  std::cout << __PRETTY_FUNCTION__ << "(" << viewRect.ul.x << "," << viewRect.ul.y << ")x("
            << viewRect.lr.x << "," << viewRect.lr.y << ")" << std::endl;

  palette->getPalette(PROGRESSBAR_BG, color);
  renderer->rectangle(temp, color);
  temp.zoom(-1, -1);
  pxcent = temp.width() * percent / 100;
  pxcent2 = temp.width() - pxcent;
  temp.width(pxcent);
  renderer->filledRectangle(temp, color);
  palette->getPalette(PROGRESSBAR_FG, color);
  temp.move(pxcent, 0);
  temp.width(pxcent2);
  renderer->filledRectangle(temp, color);
  //if (showPercent)
}

void ProgressBar::handleEvent(Event *evt)
{
  View::handleEvent(evt);
  if (evt)
  {
    Rectangle viewRect;
    getExtent(viewRect);
    std::cout << "EVT PBAR (" << viewRect.ul.x << "," << viewRect.ul.y << ")x("
              << viewRect.lr.x << "," << viewRect.lr.y << ")" << std::endl;
  }
}
