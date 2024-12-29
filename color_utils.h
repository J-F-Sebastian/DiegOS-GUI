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

#ifndef _COLOR_UTILS_H_
#define _COLOR_UTILS_H_

#include <cstdint>

struct ARGB
{
	uint8_t a, r, g, b;
};

union ARGBColor
{
	struct ARGB colorARGB;
	uint8_t colorAR[8];
	uint32_t color;
};

// TODO: this should take into account little and big endian
inline void toARGBColor(const uint32_t ARGB, union ARGBColor *clr)
{
	clr->colorARGB.b = ARGB & 0xFF;
	clr->colorARGB.g = (ARGB >> 8) & 0xFF;
	clr->colorARGB.r = (ARGB >> 16) & 0xFF;
	clr->colorARGB.a = (ARGB >> 24) & 0xFF;
}

#endif