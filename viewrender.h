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

#ifndef _VIEWRENDER_H_
#define _VIEWRENDER_H_

#include <cstdint>
#include "geometry.h"

/*
 * ViewRender is merely an interface; actual implementations are declared elsehere
 * and MUST be retrieved invoking the ViewRenderFactory object get() method.
 */

class ViewRender
{
public:
	virtual ~ViewRender(){};

	/*
	 * Trace a line from a to b with the specified color.
	 *
	 * PARAMETER IN
	 *  Point &a - reference to the starting point on screen
	 *  Point &b - reference to the ending point on screen
	 *  uint32_t color - the color to be used (bit depth depends on the renderer)
	 */
	virtual void line(const Point &a, const Point &b, uint32_t color) = 0;
	/*
	 * Trace a horizontal line of len pixels starting from a with the specified color.
	 *
	 * PARAMETER IN
	 *  Point &a - reference to the starting point on screen
	 *  int len - line length (left to right)
	 *  uint32_t color - the color to be used (bit depth depends on the renderer)
	 */
	virtual void hline(const Point &a, int len, uint32_t color) = 0;
	/*
	 * Trace a vertical line of len pixels starting from a with the specified color.
	 *
	 * PARAMETER IN
	 *  Point &a - reference to the starting point on screen
	 *  int len - line length (top to bottom)
	 *  uint32_t color - the color to be used (bit depth depends on the renderer)
	 */
	virtual void vline(const Point &a, int len, uint32_t color) = 0;
	/*
	 * Trace a rectangle using the coordinates stored in rect with the specified color.
	 *
	 * PARAMETER IN
	 *  Rectangle &rect - reference to the rectangle on screen
	 *  uint32_t color - the color to be used (bit depth depends on the renderer)
	 */
	virtual void rectangle(const Rectangle &rect, uint32_t color) = 0;
	/*
	 * Trace a rectangle using the coordinates stored in rect with the specified color.
	 * The rectangle is filled with the same color.
	 *
	 * PARAMETER IN
	 *  Rectangle &rect - reference to the rectangle on screen
	 *  uint32_t color - the color to be used (bit depth depends on the renderer)
	 */
	virtual void filledRectangle(const Rectangle &rect, uint32_t color) = 0;
	/*
	 * Trace a rectangle using the coordinates stored in rect with the specified color.
	 * The rectangle is filled with the second specified color.
	 *
	 * PARAMETER IN
	 *  Rectangle &rect - reference to the rectangle on screen
	 *  uint32_t colors[] - the colors to be used (bit depth depends on the renderer),
	 *                           first color is applied to the border, second color is used
	 *                           to fill the rectangle.
	 */
	virtual void filledRectangle2(const Rectangle &rect, uint32_t colors[2]) = 0;
	/*
	 * Write the specified text using the coordinates stored in rect with the specified color.
	 * The rectangle is filled in height and width with respect to the font size and aspect ratio.
	 *
	 * PARAMETER IN
	 *  Rectangle &rect - reference to the rectangle on screen
	 *  uint32_t color - the color to be used (bit depth depends on the renderer),
	 *  char *text - Text to be written on screen
	 */
	virtual void textBox(const char *text, Rectangle &out) = 0;
	/*
	 * Write the specified text using the coordinates stored in rect with the specified color.
	 * The rectangle is filled in height and width with respect to the font size and aspect ratio.
	 *
	 * PARAMETER IN
	 *  Rectangle &rect - reference to the rectangle on screen
	 *  uint32_t color - the color to be used (bit depth depends on the renderer),
	 *  char *text - Text to be written on screen
	 */
	virtual void text(const Rectangle &rect, uint32_t color, const char *text) = 0;
	/*
	 * Write the specified text using the coordinates stored in rect with the specified color.
	 * The rectangle is filled in height and width with respect to the font size and aspect ratio.
	 *
	 * PARAMETER IN
	 *  Rectangle &rect - reference to the rectangle on screen
	 *  uint32_t color - the color to be used (bit depth depends on the renderer),
	 *  char *text - Text to be written on screen
	 */
	virtual void textUNICODE(const Rectangle &rect, uint32_t color, const uint16_t *text) = 0;

	virtual void *loadBMP(const char *name) = 0;
	virtual bool unloadBMP(void *bmp) = 0;
	virtual void drawBMP(void *bmp, const Rectangle &rect) = 0;
	/*
	 * Show on screen the video buffer; should have no effect if the renderer use no buffering
	 * and writes directly to video memory.
	 */
	virtual void show(void) = 0;
	/*
	 * Clear the screen using the specified color.
	 *
	 * PARAMETER IN
	 *  uint32_t color - the color to be used (bit depth depends on the renderer),
	 */
	virtual void clear(uint32_t color) = 0;
	/*
	 * Creates a buffer for rendering.
	 * Callers can create as much buffers as they need - provided that there
	 * is enough free memory to allocate buffers.
	 * The renderer will use one buffer at a time, it can be set by invoking setBuffer().
	 *
	 * PARAMETER IN
	 * Rectangle &rect - reference to a rectangle describing the area of the buffer,
	 *                   it should match the extent of the object to be drawn.
	 *                   rect is expected in the form of ((0,0), (width, height)).
	 *
	 * RETURN
	 * A pointer to a buffer or NULL in case of failure.
	 */
	virtual void *createBuffer(const Rectangle &rect) = 0;
	/*
	 * Releases a buffer used for rendering.
	 * NOTE: be sure to remove the buffer from the renderer by calling
	 * setBuffer() with a different buffer or passing NULL.
	 *
	 * PARAMETER IN
	 * const void *buffer - the buffer to be released.
	 */
	virtual void releaseBuffer(const void *buffer) = 0;
	/*
	 * Set a buffer for rendering.
	 * Once a buffer is set, all drawing functions will refer to the buffer
	 * as a destination, considering coordinates and sizes.
	 * If the function is invoked passing NULL as parameter, the internal reference to
	 * the buffer will point to the screen memory.
	 * In this case all drawing will refer to the video buffer,
	 * considering coordinates and sizes.
	 * setBuffer() can be called multiple time to alternate buffers,
	 * in order to draw to multiple buffers.
	 *
	 * PARAMETER IN
	 * const void *buffer - the buffer to be used as drawing output.
	 */
	virtual void setBuffer(const void *buffer) = 0;
	/*
	 * Copy a buffer to the video memory.
	 * If you are rendering from several buffers you must be sure to call
	 * writeBuffer() for all of them before calling show().
	 *
	 * PARAMETER IN
	 * const void *buffer - the buffer to be used as input for writing to video memory.
	 * Rectangle &rect - reference to a rectangle describing the area to be copied (inside the buffer).
	 * Rectangle &vidmem - reference to a rectangle describing the area to be copied (inside the video memory).
	 */
	virtual void writeBuffer(const void *buffer, const Rectangle &rect, const Rectangle &vidmem) = 0;

protected:
	ViewRender(int xres, int yres, int bitdepth) : xres(xres), yres(yres), bitDepth(bitdepth) {}

	int xres, yres, bitDepth;
};

#endif