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

#ifndef _PALETTE_H_
#define _PALETTE_H_

#include <cstdint>

class Palette
{
public:
	Palette() : colorsNum(0) {}
	explicit Palette(unsigned num) : colorsNum(num) {}
	virtual ~Palette() {}

	virtual bool setPalette(unsigned index, unsigned color) = 0;
	virtual bool getPalette(unsigned index, unsigned &color) = 0;
	virtual bool loadPalette(const void *data) = 0;
	virtual bool storePalette(void *data) = 0;

	unsigned size(void) { return colorsNum; }

protected:
	unsigned colorsNum;
};

class PaletteTrueColor : public Palette
{
public:
	explicit PaletteTrueColor(unsigned num);
	virtual ~PaletteTrueColor();

	virtual bool setPalette(unsigned index, unsigned color) override;
	virtual bool getPalette(unsigned index, unsigned &color) override;
	virtual bool loadPalette(const void *data) override;
	virtual bool storePalette(void *data) override;

private:
	uint32_t *palette;
};

#endif