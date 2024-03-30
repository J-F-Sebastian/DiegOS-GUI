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

#ifndef _VIEW_RENDER_HW_
#define _VIEW_RENDER_HW_

#include "viewrender.h"

class ViewRenderHW : public ViewRender
{
public:
	ViewRenderHW(int xres, int yres, int bitdepth);
	virtual ~ViewRenderHW();
	virtual void line(const Point &a, const Point &b, uint32_t color) override;
	virtual void hline(const Point &a, int len, uint32_t color) override;
	virtual void vline(const Point &a, int len, uint32_t color) override;
	virtual void rectangle(const Rectangle &rect, uint32_t color) override;
	virtual void filledRectangle(const Rectangle &rect, uint32_t color) override;
	virtual void filledRectangle2(const Rectangle &rect, uint32_t colors[2]) override;
	virtual void textBox(const char *text, Rectangle &out) override;
	virtual void text(const Rectangle &rect, uint32_t color, const char *text) override;
	virtual void textUNICODE(const Rectangle &rect, uint32_t color, const uint16_t *text) override;
	virtual void *loadBMP(const char *name) override;
	virtual bool unloadBMP(void *bmp) override;
	virtual void drawBMP(void *bmp, const Rectangle &rect) override;
	virtual void show(void) override;
	virtual void clear(uint32_t color) override;
	virtual void *createBuffer(const Rectangle &rect);
	virtual void releaseBuffer(const void *buffer);
	virtual void setBuffer(const void *buffer);
	virtual void writeBuffer(const void *buffer, const Rectangle &rect, const Rectangle &vidmem) override;
};

#endif