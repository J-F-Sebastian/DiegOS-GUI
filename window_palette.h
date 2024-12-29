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

#ifndef _WINDOW_PALETTE_H_
#define _WINDOW_PALETTE_H_

/*
 * Palette for windows, bright dark and main are used to draw the enveloping frame,
 * Title colors are used for the window title bar, icons is the background of button icons
 * controlling zoom close etc.
 * Window text is the default text color for fonts printed inside the window space.
 */
enum WINDOW_PAL
{
	WINDOW_BRIGHT,
	WINDOW_DARK,
	WINDOW_MAIN,
	WINDOW_TITLE_BG,
	WINDOW_TITLE_TXT,
	WINDOW_ICONS_BG,
	WINDOW_TEXT,
	WINDOW_PAL_NUM
};

#endif