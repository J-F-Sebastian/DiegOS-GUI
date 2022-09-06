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

#ifndef _PALETTEGROUPFACTORY_H_
#define _PALETTEGROUPFACTORY_H_

#include "palettegroup.h"

enum PaletteGroupType
{
    PALETTE_WINOS2,
    PALETTE_MACINTOSH,
    PALETTE_RISCOS,
    PALETTE_DIEGOS,
    PALETTE_DEBUG
};

class PaletteGroupFactory
{
public:
    static class PaletteGroup *create(enum PaletteGroupType sel, int bitdepth);

private:
    PaletteGroupFactory();
};

#endif