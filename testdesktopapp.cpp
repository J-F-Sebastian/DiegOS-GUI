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

#include "desktopapp.h"

int main(int argc, char *argv[])
{
    DesktopApp myApp;
    Rectangle win(10, 10, 700, 400);

    myApp.createWindow(win, "My Window");
    win.width(500);
    win.move(25, 45);
    myApp.createWindow(win, "My other Window");
    win.width(400);
    win.height(200);
    win.move(150, 125);
    myApp.createWindow(win, "Next");
    myApp.run();
}