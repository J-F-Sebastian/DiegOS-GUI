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

#include "palette.h"

PaletteTrueColor::PaletteTrueColor(unsigned num) : Palette(num)
{
	palette = new uint32_t[num];
	// FIXME throw exception?
}

PaletteTrueColor::~PaletteTrueColor()
{
	delete[] palette;
}

bool PaletteTrueColor::setPalette(unsigned index, unsigned color)
{
	if (index < colorsNum)
	{
		// ARGB
		palette[index] = color & 0x00FFFFFF;
		return true;
	}
	return false;
}

bool PaletteTrueColor::getPalette(unsigned index, unsigned &color)
{
	if (index < colorsNum)
	{
		// ARGB
		color = palette[index];
		return true;
	}
	return false;
}

bool PaletteTrueColor::loadPalette(const void *data)
{
	if (!data)
		return false;

	memcpy(palette, data, colorsNum * sizeof(uint32_t));
	// ARGB
	for (unsigned i = 0; i < colorsNum; i++)
		palette[i] &= 0x00FFFFFF;
	return true;
}

bool PaletteTrueColor::storePalette(void *data)
{
	if (!data)
		return false;

	memcpy(data, palette, colorsNum * sizeof(uint32_t));
	return true;
}