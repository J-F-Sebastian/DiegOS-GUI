/*
 * DiegOS Operating System source code
 *
 * Copyright (C) 2012 - 2024 Diego Gallizioli
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

#ifndef _TEXT_BUFFER_H_
#define _TEXT_BUFFER_H_

#include <cstdint>

class TextBuffer
{
public:
	TextBuffer(int xres, int yres);
	~TextBuffer();

	uint16_t *getBuffer(void) const { return buffer; }

	void setBuffer(uint16_t value);

	void setBufferXY(int x, int y, uint16_t value);
	void setBufferXY(int x, int y, uint16_t *value, int len);

	uint16_t getBufferXY(int x, int y);

	virtual void blitBuffer(int srcx, int srcy, int dstx, int dsty, char *dest) {};

private:
	uint16_t *buffer;
	int bufferSize;
	int xres, yres;
};

#endif