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

#ifndef _PALETTEGROUP_H_
#define _PALETTEGROUP_H_

#include "palette.h"

class PaletteGroup
{
public:
	enum
	{
		PAL_FRAME,
		PAL_TITLEBAR,
		PAL_PROGRESSBAR,
		PAL_BACKGROUND,
		PAL_ICONS,
		PAL_MENU,
		PAL_POPUP,
		PAL_BUTTON,
		PAL_LIST,
		PAL_DESKTOP,
		PAL_WINICON,
		PAL_SCROLLBAR,
		PAL_NUM
	};

	PaletteGroup() : group(nullptr), palettesNum(0) {}
	explicit PaletteGroup(Palette *items[], unsigned numOfPalettes);
	virtual ~PaletteGroup();

	Palette *getPalette(unsigned index);
	bool setPalette(unsigned index, Palette *palette);
	bool loadPalette(Palette *items[]);
	bool storePalette(Palette *items[]);

private:
	Palette **group;
	unsigned palettesNum;
};

#endif