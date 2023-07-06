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

#include <cstring>

#include "palettegroup.h"

PaletteGroup::PaletteGroup(Palette *items[], unsigned numOfPalettes) : palettesNum(numOfPalettes)
{
	group = new Palette *[palettesNum];

	memcpy(group, items, palettesNum * sizeof(Palette *));
}

PaletteGroup::~PaletteGroup()
{
	delete[] group;
}

Palette *PaletteGroup::getPalette(unsigned index)
{
	if (index < palettesNum)
		return group[index];

	return nullptr;
}

bool PaletteGroup::setPalette(unsigned index, Palette *palette)
{
	if (palette && (index < palettesNum))
	{
		group[index] = palette;
		return true;
	}

	return false;
}

bool PaletteGroup::loadPalette(Palette *items[])
{
	if (items == nullptr)
		return false;

	for (unsigned i = 0; i < palettesNum; i++)
	{
		if (items[i] == nullptr)
			return false;
	}

	if (group)
		delete[] group;

	group = new Palette *[palettesNum];

	memcpy(group, items, palettesNum * sizeof(Palette *));
	return true;
}

bool PaletteGroup::storePalette(Palette *items[])
{
	if (items == nullptr)
		return false;
	memcpy(items, group, palettesNum * sizeof(Palette *));
	return true;
}