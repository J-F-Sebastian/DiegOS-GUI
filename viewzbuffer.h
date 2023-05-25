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

#ifndef _VIEWZBUFFER_H_
#define _VIEWZBUFFER_H_

#include <cstdint>
#include "geometry.h"

/*
 * ViewZBuffer is a class managing a Z-Buffer for rendering or drawing to screen.
 * The buffer need to be initialized with the screen resolution, then it can be
 * reset, set for specific areas, and tested for truth.
 * All areas passed to the methods MUST BE in screen coordinates, i.e. globalized.
 */
class ViewZBuffer
{
public:
	static ViewZBuffer *instance(void);
	~ViewZBuffer();

	void configure(Rectangle &mainscreen);

	void set(Rectangle &area);
	void clear(Rectangle &area);
	void clear(void);
	bool isAreaSet(Rectangle &area);
	bool isAreaClear(Rectangle &area);

private:
	ViewZBuffer();

	Rectangle screen;
	// uint32_t *buffer;
	uint8_t *buffer;
	// int width;
};

#endif