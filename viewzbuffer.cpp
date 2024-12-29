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

#include <cstring>

#include "viewzbuffer.h"

ViewZBuffer *ViewZBuffer::instance()
{
	static ViewZBuffer obj;
	return &obj;
}

void ViewZBuffer::configure(Rectangle &mainscreen)
{
	screen = mainscreen;

	if (buffer)
		delete buffer;

	buffer = new uint8_t[screen.width() * screen.height()];
	memset(buffer, 0, screen.width() * screen.height());
}

ViewZBuffer::ViewZBuffer() : screen(0, 0, 0, 0), buffer(nullptr)
{
}

ViewZBuffer::~ViewZBuffer()
{
	if (buffer)
		delete buffer;
}

void ViewZBuffer::set(Rectangle &area)
{
	int y, base;

	if (screen.includes(area))
	{
		/*
		 * base is the starting pointer inside the buffer.
		 */
		base = area.ul.y * screen.width() + area.ul.x;

		for (y = 0; y < area.height(); y++)
		{
			memset(buffer + base, 1, area.width());
			base += screen.width();
		}
	}
}

void ViewZBuffer::clear(Rectangle &area)
{
	int y, base;

	if (screen.includes(area))
	{
		/*
		 * base is the starting pointer inside the buffer.
		 */
		base = area.ul.y * screen.width() + area.ul.x;

		for (y = 0; y < area.height(); y++)
		{
			memset(buffer + base, 0, area.width());
			base += screen.width();
		}
	}
}

void ViewZBuffer::clear()
{
	memset(buffer, 0, screen.width() * screen.height());
}

bool ViewZBuffer::isAreaSet(Rectangle &area)
{
	int x, y, base;

	if (screen.includes(area))
	{
		/*
		 * base is the starting pointer inside the buffer.
		 */
		base = area.ul.y * screen.width() + area.ul.x;

		for (y = 0; y < area.height(); y++)
		{
			for (x = 0; x < area.width(); x++)
				if (buffer[base++] == 0)
					return false;

			base += screen.width() - area.width();
		}
	}

	return true;
}

bool ViewZBuffer::isAreaClear(Rectangle &area)
{
	int x, y, base;

	if (screen.includes(area))
	{
		/*
		 * base is the starting pointer inside the buffer.
		 */
		base = area.ul.y * screen.width() + area.ul.x;

		for (y = 0; y < area.height(); y++)
		{
			for (x = 0; x < area.width(); x++)
				if (buffer[base++] == 1)
					return false;

			base += screen.width() - area.width();
		}
	}

	return true;
}

#if 0
void ViewZBuffer::configure(Rectangle &mainscreen)
{
	screen = mainscreen;

	width = screen.width() / 32;
	if (screen.width() % 32)
		width++;

	if (buffer)
		delete buffer;
	buffer = new uint32_t[width * screen.height()];
}

ViewZBuffer::ViewZBuffer() : screen(0, 0, 0, 0), buffer(nullptr), width(0)
{
}

ViewZBuffer::~ViewZBuffer()
{
	if (buffer)
		delete buffer;
}

void ViewZBuffer::set(Rectangle &area)
{
	int run, x, y, base;
	uint32_t prologue, epilogue;

	if (screen.includes(area))
	{
		/*
		 * base is the starting pointer inside the buffer.
		 */
		base = area.ul.y * width + area.ul.x;

		/*
		 * run is the number of buffer cells to be set to (-1UL) excluding prologue
		 * and epilogue.
		 * Prologue is 0 if area.ul.x is multiple of 32, run will take care of it.
		 */
		run = area.width();
		if (area.ul.x % 32)
			run -= 32 - area.ul.x % 32;
		run -= area.lr.x % 32;
		run /= 32;

		/*
		 * the prologue is the bit string starting the sequence,
		 * it can be 0 if area.ul.x is multiple of 32, otherwise
		 * it can be set to 2 different logical values
		 *
		 * MSB        LSB    The area covers the whole buffer cell
		 * 111110...00000
		 *
		 * MSB        LSB    The area does not cover the whole buffer cell
		 * 000011110..000
		 *
		 * Last case implies area.ul.x % 32 + area.width() < 32.
		 */
		prologue = area.ul.x % 32;
		if (prologue)
		{
			prologue = (-1UL) << prologue;
			if (area.ul.x % 32 + area.width() < 32)
				prologue &= (-1UL) >> (32 - area.ul.x % 32 - area.width());
		}

		/*
		 * The epilogue is the bit string terminating the sequence.
		 * It can be 0 if area.lr.x is multiple of 32.
		 */
		epilogue = area.lr.x % 32;
		if (epilogue)
			epilogue = (-1UL) >> (32 - epilogue);

		for (y = 0; y < area.height(); y++)
		{
			if (prologue)
				buffer[base++] |= prologue;
			for (x = 0; x < run; x++)
				buffer[base++] = (-1UL);
			if (epilogue)
				buffer[base++] |= epilogue;
			base += width - area.width();
		}
	}
}

void ViewZBuffer::clear(Rectangle &area)
{
	int run, x, y, base;
	uint32_t prologue, epilogue;

	if (screen.includes(area))
	{
		/*
		 * base is the starting pointer inside the buffer.
		 */
		base = area.ul.y * width + area.ul.x;

		/*
		 * run is the number of buffer cells to be set to (-1UL) excluding prologue
		 * and epilogue.
		 * Prologue is 0 if area.ul.x is multiple of 32, run will take care of it.
		 */
		run = area.width();
		if (area.ul.x % 32)
			run -= 32 - area.ul.x % 32;
		run -= area.lr.x % 32;
		run /= 32;

		/*
		 * the prologue is the bit string starting the sequence,
		 * it can be 0 if area.ul.x is multiple of 32, otherwise
		 * it can be set to 2 different logical values
		 *
		 * MSB        LSB    The area covers the whole buffer cell
		 * 111110...00000
		 *
		 * MSB        LSB    The area does not cover the whole buffer cell
		 * 000011110..000
		 *
		 * Last case implies area.ul.x % 32 + area.width() < 32.
		 */
		prologue = area.ul.x % 32;
		if (prologue)
		{
			prologue = (-1UL) << prologue;
			if (area.ul.x % 32 + area.width() < 32)
				prologue &= (-1UL) >> (32 - area.ul.x % 32 - area.width());
		}

		/*
		 * The epilogue is the bit string terminating the sequence.
		 * It can be 0 if area.lr.x is multiple of 32.
		 */
		epilogue = area.lr.x % 32;
		if (epilogue)
			epilogue = (-1UL) >> (32 - epilogue);

		for (y = 0; y < area.height(); y++)
		{
			if (prologue)
				buffer[base++] &= ~prologue;
			for (x = 0; x < run; x++)
				buffer[base++] = 0;
			if (epilogue)
				buffer[base++] &= ~epilogue;
			base += width - area.width();
		}
	}
}

bool ViewZBuffer::isAreaSet(Rectangle &area)
{
	int x, y, base;

	if (screen.includes(area))
	{
		base = area.ul.y * screen.width() + area.ul.x;
		for (y = 0; y < area.height(); y++)
		{
			for (x = 0; x < area.width(); x++)
				if (buffer[base++] == 0)
					return false;

			base += screen.width() - area.width();
		}
	}

	return true;
}

bool ViewZBuffer::isAreaClear(Rectangle &area)
{
	int x, y, base;

	if (screen.includes(area))
	{
		base = area.ul.y * screen.width() + area.ul.x;
		for (y = 0; y < area.height(); y++)
		{
			for (x = 0; x < area.width(); x++)
				if (buffer[base++])
					return false;

			base += screen.width() - area.width();
		}
	}

	return true;
}

bool ViewZBuffer::isAreaLowerThan(Rectangle &area, uint16_t value)
{
	int x, y, base;

	if (screen.includes(area))
	{
		base = area.ul.y * screen.width() + area.ul.x;
		for (y = 0; y < area.height(); y++)
		{
			for (x = 0; x < area.width(); x++)
				if (buffer[base++] >= value)
					return false;

			base += screen.width() - area.width();
		}
	}

	return true;
}
#endif
