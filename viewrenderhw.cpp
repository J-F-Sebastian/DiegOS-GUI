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

static inline void to_SDL_Rect(const Rectangle &rect, SDL_Rect &srect)
{
	srect.x = rect.ul.x;
	srect.y = rect.ul.y;
	srect.w = rect.width();
	srect.h = rect.height();
}

// The window we'll be rendering to
static SDL_Window *window = NULL;
// The renderer
static SDL_Renderer *renderer = NULL;
// The texture format
static Uint32 textureFormat = 0;
// The font
static TTF_Font *font = NULL;

ViewRenderHW::ViewRenderHW(int xres, int yres, int bitdepth) : ViewRender(xres, yres, bitdepth)
{
	SDL_RendererInfo info;

	// Initialize SDL
	if (SDL_InitSubSystem(SDL_INIT_VIDEO) < 0)
	{
		std::cout << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
		return;
	}

	// Create window
	window = SDL_CreateWindow("GUI", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, xres, yres, SDL_WINDOW_OPENGL);
	if (window == NULL)
	// if (SDL_CreateWindowAndRenderer(xres, yres, 0, &window, &renderer))
	//  if (SDL_CreateWindowAndRenderer(xres, yres, SDL_WINDOW_FULLSCREEN, &window, &renderer))
	{
		std::cout << "Window could not be created! SDL_Error: " << SDL_GetError() << std::endl;
		return;
	}

	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	if (renderer == NULL)
	{
		std::cout << "Renderer could not be created! SDL_Error: " << SDL_GetError() << std::endl;
		SDL_DestroyWindow(window);
		return;
	}

	if (SDL_GetRendererInfo(renderer, &info))
	{
		std::cout << "Could not get Renderer info! SDL_Error: " << SDL_GetError() << std::endl;
		SDL_DestroyRenderer(renderer);
		SDL_DestroyWindow(window);
		return;
	}

	if ((info.flags & SDL_RENDERER_TARGETTEXTURE) == 0)
	{
		std::cout << "Renderer cannot render to textures!" << std::endl;
		SDL_DestroyRenderer(renderer);
		SDL_DestroyWindow(window);
		return;
	}

	std::cout << "RENDERER: " << info.name << std::endl;
	std::cout << "TEXTURE FORMATS: " << info.num_texture_formats << std::endl;
	for (unsigned i = 0; i < info.num_texture_formats; i++)
	{
		std::cout << '(' << i << ')' << ' ' << SDL_GetPixelFormatName(info.texture_formats[i])
			  << " pixel type " << SDL_PIXELTYPE(info.texture_formats[i])
			  << " bits per pixel " << SDL_BITSPERPIXEL(info.texture_formats[i]);

		if (textureFormat == 0)
		{
			if (SDL_BITSPERPIXEL(info.texture_formats[i]) == (Uint32)bitDepth)
			{
				textureFormat = info.texture_formats[i];
				std::cout << " <---";
			}
		}

		std::cout << std::endl;
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
	union ARGBColor c;
	toARGBColor(color, &c);
	SDL_Rect srect;
	to_SDL_Rect(rect, srect);
	SDL_SetRenderDrawColor(renderer, c.colorARGB.r, c.colorARGB.g, c.colorARGB.b, SDL_ALPHA_OPAQUE);
	SDL_RenderDrawRect(renderer, &srect);
}

void ViewRenderHW::filledRectangle(const Rectangle &rect, uint32_t color)
{
	union ARGBColor c;
	toARGBColor(color, &c);
	SDL_Rect srect;
	to_SDL_Rect(rect, srect);
	SDL_SetRenderDrawColor(renderer, c.colorARGB.r, c.colorARGB.g, c.colorARGB.b, SDL_ALPHA_OPAQUE);
	SDL_RenderFillRect(renderer, &srect);
}

void ViewRenderHW::filledRectangle2(const Rectangle &rect, uint32_t colors[2])
{
	union ARGBColor c[2];
	toARGBColor(colors[0], &c[0]);
	toARGBColor(colors[1], &c[1]);
	SDL_Rect srect;
	to_SDL_Rect(rect, srect);
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
	if (!text)
		return;

	union ARGBColor c;
	toARGBColor(color, &c);
	SDL_Rect srect;
	to_SDL_Rect(rect, srect);
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
	if (!text)
		return;

	union ARGBColor c;
	toARGBColor(color, &c);
	SDL_Rect srect;
	to_SDL_Rect(rect, srect);
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
		SDL_Rect srect;
		to_SDL_Rect(rect, srect);
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
	SDL_SetRenderDrawColor(renderer,
			       c.colorARGB.r,
			       c.colorARGB.g,
			       c.colorARGB.b,
			       SDL_ALPHA_OPAQUE);
	SDL_RenderClear(renderer);
}

void *ViewRenderHW::createBuffer(const Rectangle &rect)
{
	SDL_Texture *texture = SDL_CreateTexture(renderer,
						 textureFormat,
						 SDL_TEXTUREACCESS_TARGET,
						 rect.width(),
						 rect.height());

	if (texture)
	{
		if (SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_NONE))
		{
			std::cout << "Renderer error!  SDL_Error: " << SDL_GetError() << std::endl;
			SDL_DestroyTexture(texture);
			return NULL;
		}
	}
	return (void *)texture;
}

void ViewRenderHW::releaseBuffer(const void *buffer)
{
	if (buffer)
		SDL_DestroyTexture((SDL_Texture *)buffer);
}

void ViewRenderHW::setBuffer(const void *buffer)
{
	if (SDL_SetRenderTarget(renderer, (SDL_Texture *)buffer))
		std::cout << "Renderer error!  SDL_Error: " << SDL_GetError() << std::endl;
}

void ViewRenderHW::writeBuffer(const void *buffer, const Rectangle &rect)
{
	A_RECT;

	if (SDL_RenderCopy(renderer, (SDL_Texture *)buffer, NULL, &srect))
		std::cout << "Renderer error!  SDL_Error: " << SDL_GetError() << std::endl;
}