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

#include <iostream>
#include "viewrenderhw.h"
#include "color_utils.h"
#include "SDL.h"
#include "SDL_ttf.h"

// Yes a MACRO to avoid repeated adjustments ...
#define A_RECT \
	SDL_Rect srect = {.x = rect.ul.x, .y = rect.ul.y, .w = rect.width() + 1, .h = rect.height() + 1}

// The window we'll be rendering to
static SDL_Window *window = NULL;
// The renderer
static SDL_Renderer *renderer = NULL;
// The font
static TTF_Font *font = NULL;

ViewRenderHW::ViewRenderHW(int xres, int yres, int bitdepth) : ViewRender(xres, yres, bitdepth)
{
	// Initialize SDL
	if (SDL_InitSubSystem(SDL_INIT_VIDEO) < 0)
	{
		std::cout << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
		return;
	}

	// Create window
	if (SDL_CreateWindowAndRenderer(xres, yres, 0, &window, &renderer))
	// if (SDL_CreateWindowAndRenderer(xres, yres, SDL_WINDOW_FULLSCREEN, &window, &renderer))
	{
		std::cout << "Window could not be created! SDL_Error: " << SDL_GetError() << std::endl;
		return;
	}

	// Initialize Font
	if (TTF_Init())
	{
		std::cout << "Font could not be inited! SDL_Error: " << TTF_GetError() << std::endl;
	}

	font = TTF_OpenFont("C:\\windows\\fonts\\segoeui.ttf", 14);
	if (!font)
	{
		std::cout << "Font could not be loaded! SDL_Error: " << TTF_GetError() << std::endl;
	}

	SDL_RenderClear(renderer);
}

ViewRenderHW::~ViewRenderHW()
{
	if (renderer && window)
	{
		SDL_DestroyRenderer(renderer);
		SDL_DestroyWindow(window);
	}
	if (font)
		TTF_CloseFont(font);

	renderer = NULL;
	window = NULL;
	font = NULL;

	if (TTF_WasInit())
		TTF_Quit();
	if (SDL_WasInit(SDL_INIT_VIDEO))
		SDL_QuitSubSystem(SDL_INIT_VIDEO);
}

void ViewRenderHW::line(const Point &a, const Point &b, uint32_t color)
{
	union ARGBColor c;
	toARGBColor(color, &c);
	SDL_SetRenderDrawColor(renderer, c.colorARGB.r, c.colorARGB.g, c.colorARGB.b, SDL_ALPHA_OPAQUE);
	SDL_RenderDrawLine(renderer, a.x, a.y, b.x, b.y);
}

void ViewRenderHW::hline(const Point &a, int len, uint32_t color)
{
	union ARGBColor c;
	toARGBColor(color, &c);
	SDL_SetRenderDrawColor(renderer, c.colorARGB.r, c.colorARGB.g, c.colorARGB.b, SDL_ALPHA_OPAQUE);
	SDL_RenderDrawLine(renderer, a.x, a.y, a.x + len, a.y);
}

void ViewRenderHW::vline(const Point &a, int len, uint32_t color)
{
	union ARGBColor c;
	toARGBColor(color, &c);
	SDL_SetRenderDrawColor(renderer, c.colorARGB.r, c.colorARGB.g, c.colorARGB.b, SDL_ALPHA_OPAQUE);
	SDL_RenderDrawLine(renderer, a.x, a.y, a.x, a.y + len);
}

void ViewRenderHW::rectangle(const Rectangle &rect, uint32_t color)
{
	A_RECT;

	union ARGBColor c;
	toARGBColor(color, &c);
	SDL_SetRenderDrawColor(renderer, c.colorARGB.r, c.colorARGB.g, c.colorARGB.b, SDL_ALPHA_OPAQUE);
	SDL_RenderDrawRect(renderer, &srect);
}

void ViewRenderHW::filledRectangle(const Rectangle &rect, uint32_t color)
{
	A_RECT;

	union ARGBColor c;
	toARGBColor(color, &c);
	SDL_SetRenderDrawColor(renderer, c.colorARGB.r, c.colorARGB.g, c.colorARGB.b, SDL_ALPHA_OPAQUE);
	SDL_RenderFillRect(renderer, &srect);
}

void ViewRenderHW::filledRectangle2(const Rectangle &rect, uint32_t colors[2])
{
	A_RECT;

	union ARGBColor c[2];
	toARGBColor(colors[0], &c[0]);
	toARGBColor(colors[1], &c[1]);
	SDL_SetRenderDrawColor(renderer, c[0].colorARGB.r, c[0].colorARGB.g, c[0].colorARGB.b, SDL_ALPHA_OPAQUE);
	SDL_RenderDrawRect(renderer, &srect);
	srect.x++;
	srect.y++;
	srect.w -= 2;
	srect.h -= 2;
	SDL_SetRenderDrawColor(renderer, c[1].colorARGB.r, c[1].colorARGB.g, c[1].colorARGB.b, SDL_ALPHA_OPAQUE);
	SDL_RenderFillRect(renderer, &srect);
}

void ViewRenderHW::textBox(const char *text, Rectangle &out)
{
	int w, h;

	out.ul.x = out.ul.y = 0;
	if (!text || TTF_SizeText(font, text, &w, &h))
	{
		out.lr.x = out.lr.y = 0;
	}
	else
	{
		out.lr.x = w;
		out.lr.y = h;
	}
}

void ViewRenderHW::text(const Rectangle &rect, uint32_t color, const char *text)
{
	A_RECT;

	if (!text)
		return;

	union ARGBColor c;
	toARGBColor(color, &c);
	SDL_Color clor;
	clor.a = c.colorARGB.a;
	clor.b = c.colorARGB.b;
	clor.g = c.colorARGB.g;
	clor.r = c.colorARGB.r;

	SDL_Surface *surface = TTF_RenderText_Solid(font, text, clor);
	SDL_Texture *message = SDL_CreateTextureFromSurface(renderer, surface);
	SDL_RenderCopy(renderer, message, NULL, &srect);
	SDL_FreeSurface(surface);
	SDL_DestroyTexture(message);
}

void ViewRenderHW::textUNICODE(const Rectangle &rect, uint32_t color, const uint16_t *text)
{
	A_RECT;

	if (!text)
		return;

	union ARGBColor c;
	toARGBColor(color, &c);
	SDL_Color clor;
	clor.a = c.colorARGB.a;
	clor.b = c.colorARGB.b;
	clor.g = c.colorARGB.g;
	clor.r = c.colorARGB.r;

	SDL_Surface *surface = TTF_RenderUNICODE_Solid(font, text, clor);
	SDL_Texture *message = SDL_CreateTextureFromSurface(renderer, surface);
	SDL_RenderCopy(renderer, message, NULL, &srect);
	SDL_FreeSurface(surface);
	SDL_DestroyTexture(message);
}

void *ViewRenderHW::loadBMP(const char *name)
{
	SDL_Surface *surf = SDL_LoadBMP(name);
	if (!surf)
		return nullptr;

	SDL_Texture *bmp = SDL_CreateTextureFromSurface(renderer, surf);
	SDL_FreeSurface(surf);

	return static_cast<void *>(bmp);
}

bool ViewRenderHW::unloadBMP(void *bmp)
{
	SDL_Texture *mybmp = reinterpret_cast<SDL_Texture *>(bmp);

	if (mybmp)
	{
		SDL_DestroyTexture(mybmp);
		return true;
	}

	return false;
}

void ViewRenderHW::drawBMP(void *bmp, const Rectangle &rect)
{
	SDL_Texture *mybmp = reinterpret_cast<SDL_Texture *>(bmp);

	if (mybmp)
	{
		A_RECT;
		SDL_RenderCopy(renderer, mybmp, NULL, &srect);
	}
}

void ViewRenderHW::show()
{
	// Update the surface
	SDL_RenderPresent(renderer);
}

void ViewRenderHW::clear(uint32_t color)
{
	union ARGBColor c;
	toARGBColor(color, &c);
	SDL_SetRenderDrawColor(renderer, c.colorARGB.r, c.colorARGB.g, c.colorARGB.b, SDL_ALPHA_OPAQUE);
	SDL_RenderClear(renderer);
}