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

#ifndef _VIEWRENDERINSTANCE_H_
#define _VIEWRENDERINSTANCE_H_

#include "viewrenderfactory.h"

class ViewRenderInstance
{
public:
	/*
	 * Configure the system renderer with the specified type, resolution and bit depth.
	 * This method can be invoked runtime at will to change the renderer, provided that
	 * the renderer instance can run on the selected hardware/os.
	 */
	void configure(enum ViewRenderType sel, int xres, int yres, int bitdepth);

	/*
	 * Retrieve the configured PaletteGroup object.
	 */
	class ViewRender *get();

	/*
	 * Retrieve the singleton instance.
	 */
	static class ViewRenderInstance *instance();

private:
	class ViewRender *renderer;

	ViewRenderInstance();
};

#endif