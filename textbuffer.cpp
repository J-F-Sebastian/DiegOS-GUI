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

#include "textbuffer.h"

TextBuffer::TextBuffer(int xres, int yres) : xres(xres), yres(yres)
{
	bufferSize = xres * yres;
	buffer = new uint16_t[bufferSize];
}

TextBuffer::~TextBuffer()
{
	delete[] buffer;
}

void TextBuffer::setBuffer(uint16_t value)
{
	int counter;
	unsigned *destination = (unsigned *)(buffer);
	unsigned temp = value;
	temp |= (temp << 16);

	for (counter = bufferSize; counter > 1; counter -= 2)
		*destination++ = temp;

	if (counter)
	{
		buffer[bufferSize - 1] = value;
	}
}

void TextBuffer::setBufferXY(int x, int y, uint16_t value)
{
	int offset = y * xres + x;

	if (offset < bufferSize)
		buffer[offset] = value;
}

void TextBuffer::setBufferXY(int x, int y, uint16_t *value, int len)
{
	int offset = y * xres + x;

	while (len && (offset < bufferSize))
	{
		--len;
		buffer[offset++] = *value++;
	}
}

uint16_t TextBuffer::getBufferXY(int x, int y)
{
	int offset = y * xres + x;

	if (offset < bufferSize)
		return (buffer[offset]);
	return 0;
}