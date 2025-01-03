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

#include <SDL2/SDL.h>
#include "viewinstances.h"
#include "desktopapp.h"
#include "button.h"
#include "progressbar.h"
#include "window.h"
#include "viewexec.h"
#include "eventqueue.h"
#include "viewzbuffer.h"
#include "palettetab.h"

// Screen dimension constants
static const int SCREEN_WIDTH = 1280;
static const int SCREEN_HEIGHT = 720;

DesktopApp::DesktopApp()
{
	SDL_Init(0);

	Rectangle master(0, 0, SCREEN_WIDTH - 1, SCREEN_HEIGHT - 1);
	ViewRenderInstance::instance()->configure(VRENDER_HW, SCREEN_WIDTH, SCREEN_HEIGHT, 32);
	ViewZBuffer::instance()->configure(master);
	he = ViewEventFactory::create(EST_SDL);
	SystemPaletteInstance::instance()->configure(SYS_PALETTE_DIEGOS, 32);
	PaletteGroupInstance::instance()->configure(SYS_PALETTE_DIEGOS, 32);

	app = new ViewApplication(master, he);
	app->initDesktop();
}

Window *DesktopApp::createWindow(Rectangle &viewLimits, const char *title)
{
	/* Relative to Window origin */
	Rectangle buttonLimits(25, 50, 125, 100);

	Window *newWindow = new Window(viewLimits, title, app);
	Button *newButton = new Button(buttonLimits);
	newWindow->insert(newButton);
	buttonLimits.move(25, 25);
	ProgressBar *newProgBar = new ProgressBar(buttonLimits, true);
	newWindow->insert(newProgBar);
	buttonLimits.move(0, 75);
	buttonLimits.width(buttonLimits.width() * 2);
	PaletteTab *newPalTab = new PaletteTab(buttonLimits);
	newWindow->insert(newPalTab);
	app->insert(newWindow);
	return newWindow;
}

void DesktopApp::run()
{
	app->run();
}

DesktopApp::~DesktopApp()
{
	delete app;
	delete he;
	SDL_Quit();
}