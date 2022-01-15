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

#include "palettegroupfactory.h"
#include "viewrenderhw.h"
#include <cstring>

#define BLACK 0x000000
#define MAROON 0x800000
#define GREEN 0x008000
#define OLIVE 0x808000
#define NAVY 0x000080
#define PURPLE 0x800080
#define TEAL 0x008080
#define SILVER 0xC0C0C0

#define GRAY 0x808080
#define RED 0xFF0000
#define LIME 0x00FF00
#define YELLOW 0xFFFF00
#define BLUE 0x0000FF
#define FUCHSIA 0xFF00FF
#define AQUA 0x00FFFF
#define WHITE 0xFFFFFF

#define ORANGE 0xFFA500
#define MAGENTA 0xFF00FF
#define CYAN 0x00FFFF
#define DARKGREEN 0x006400
#define BROWN 0xA52A2A
#define TAN 0xD2B48C
#define LIGHTGREY 0xD3D3D3
#define DARKGREY 0xA9A9A9

// Palette set for  PALETTE_WINOS2
static const unsigned framePaletteWINOS2[] = {WHITE, BLACK, SILVER};                      // Bright, dark, main
static const unsigned titlebarPaletteWINOS2[] = {BLUE, WHITE};                            // Background, text
static const unsigned backgroundPaletteWINOS2[] = {SILVER};                               // Background
static const unsigned progressbarPaletteWINOS2[] = {BLACK, BLUE, WHITE};                  // Background, Foreground, text
static const unsigned desktopPaletteWINOS2[] = {GRAY};                                    // Desktop background
static const unsigned buttonPaletteWINOS2[] = {WHITE, BLACK, SILVER, RED, BLACK, YELLOW}; // Bright, Dark, main, selected, disabled, pressed
static const unsigned winiconPaletteWINOS2[] = {WHITE, BLACK, SILVER, RED, BLACK};        // Bright, Dark, main, pressed, released

// Palette set for  PALETTE_DEBUG
static const unsigned framePaletteDBG[] = {RED, GREEN, BLUE};                          // Bright, dark, main
static const unsigned titlebarPaletteDBG[] = {YELLOW, WHITE};                          // Background, text
static const unsigned backgroundPaletteDBG[] = {AQUA};                                 // Background
static const unsigned progressbarPaletteDBG[] = {BLACK, BLUE, WHITE};                  // Background, Foreground, text
static const unsigned desktopPaletteDBG[] = {BLACK};                                   // Desktop background
static const unsigned buttonPaletteDBG[] = {WHITE, BLACK, SILVER, RED, BLACK, YELLOW}; // Bright, Dark, main, selected, disabled, pressed
static const unsigned winiconPaletteDBG[] = {WHITE, BLACK, SILVER, RED, BLACK};        // Bright, Dark, main, pressed, released

// Palette set for  PALETTE_MACINTOSH
static const unsigned framePaletteMAC[] = {LIGHTGREY, DARKGREY, GRAY};                 // Bright, dark, main
static const unsigned titlebarPaletteMAC[] = {WHITE, BLACK};                           // Background, text
static const unsigned backgroundPaletteMAC[] = {WHITE};                                // Background
static const unsigned progressbarPaletteMAC[] = {LIGHTGREY, DARKGREY, WHITE};          // Background, Foreground, text
static const unsigned desktopPaletteMAC[] = {GRAY};                                    // Desktop background
static const unsigned buttonPaletteMAC[] = {WHITE, BLACK, SILVER, RED, BLACK, YELLOW}; // Bright, Dark, main, selected, disabled, pressed
static const unsigned winiconPaletteMAC[] = {WHITE, BLACK, SILVER, RED, BLACK};        // Bright, Dark, main, pressed, released

//PALETTE_MACINTOSH, PALETTE_RISCOS
class PaletteGroup *PaletteGroupFactory::create(enum PaletteGroupType sel, int bitdepth)
{
    /*
     * Right now supports only 32 bit depth
     */
    if (bitdepth != 32)
        return nullptr;

    Palette *pal[PaletteGroup::PAL_NUM];
    memset(pal, 0, sizeof(pal));

    switch (sel)
    {
    case PALETTE_WINOS2:
        pal[PaletteGroup::PAL_BACKGROUND] = new PaletteTrueColor(1);
        pal[PaletteGroup::PAL_BACKGROUND]->loadPalette(backgroundPaletteWINOS2);
        pal[PaletteGroup::PAL_FRAME] = new PaletteTrueColor(3);
        pal[PaletteGroup::PAL_FRAME]->loadPalette(framePaletteWINOS2);
        pal[PaletteGroup::PAL_TITLEBAR] = new PaletteTrueColor(2);
        pal[PaletteGroup::PAL_TITLEBAR]->loadPalette(titlebarPaletteWINOS2);
        pal[PaletteGroup::PAL_PROGRESSBAR] = new PaletteTrueColor(3);
        pal[PaletteGroup::PAL_PROGRESSBAR]->loadPalette(progressbarPaletteWINOS2);
        pal[PaletteGroup::PAL_DESKTOP] = new PaletteTrueColor(1);
        pal[PaletteGroup::PAL_DESKTOP]->loadPalette(desktopPaletteWINOS2);
        pal[PaletteGroup::PAL_BUTTON] = new PaletteTrueColor(6);
        pal[PaletteGroup::PAL_BUTTON]->loadPalette(buttonPaletteWINOS2);
        pal[PaletteGroup::PAL_WINICON] = new PaletteTrueColor(5);
        pal[PaletteGroup::PAL_WINICON]->loadPalette(winiconPaletteWINOS2);
        break;
    case PALETTE_MACINTOSH:
        pal[PaletteGroup::PAL_BACKGROUND] = new PaletteTrueColor(1);
        pal[PaletteGroup::PAL_BACKGROUND]->loadPalette(backgroundPaletteMAC);
        pal[PaletteGroup::PAL_FRAME] = new PaletteTrueColor(3);
        pal[PaletteGroup::PAL_FRAME]->loadPalette(framePaletteMAC);
        pal[PaletteGroup::PAL_TITLEBAR] = new PaletteTrueColor(2);
        pal[PaletteGroup::PAL_TITLEBAR]->loadPalette(titlebarPaletteMAC);
        pal[PaletteGroup::PAL_PROGRESSBAR] = new PaletteTrueColor(3);
        pal[PaletteGroup::PAL_PROGRESSBAR]->loadPalette(progressbarPaletteMAC);
        pal[PaletteGroup::PAL_DESKTOP] = new PaletteTrueColor(1);
        pal[PaletteGroup::PAL_DESKTOP]->loadPalette(desktopPaletteMAC);
        pal[PaletteGroup::PAL_BUTTON] = new PaletteTrueColor(6);
        pal[PaletteGroup::PAL_BUTTON]->loadPalette(buttonPaletteMAC);
        pal[PaletteGroup::PAL_WINICON] = new PaletteTrueColor(5);
        pal[PaletteGroup::PAL_WINICON]->loadPalette(winiconPaletteMAC);
        break;
    case PALETTE_RISCOS:
        return nullptr;

    case PALETTE_DEBUG:
        pal[PaletteGroup::PAL_BACKGROUND] = new PaletteTrueColor(1);
        pal[PaletteGroup::PAL_BACKGROUND]->loadPalette(backgroundPaletteDBG);
        pal[PaletteGroup::PAL_FRAME] = new PaletteTrueColor(3);
        pal[PaletteGroup::PAL_FRAME]->loadPalette(framePaletteDBG);
        pal[PaletteGroup::PAL_TITLEBAR] = new PaletteTrueColor(2);
        pal[PaletteGroup::PAL_TITLEBAR]->loadPalette(titlebarPaletteDBG);
        pal[PaletteGroup::PAL_PROGRESSBAR] = new PaletteTrueColor(3);
        pal[PaletteGroup::PAL_PROGRESSBAR]->loadPalette(progressbarPaletteDBG);
        pal[PaletteGroup::PAL_DESKTOP] = new PaletteTrueColor(1);
        pal[PaletteGroup::PAL_DESKTOP]->loadPalette(desktopPaletteDBG);
        pal[PaletteGroup::PAL_BUTTON] = new PaletteTrueColor(6);
        pal[PaletteGroup::PAL_BUTTON]->loadPalette(buttonPaletteDBG);
        pal[PaletteGroup::PAL_WINICON] = new PaletteTrueColor(5);
        pal[PaletteGroup::PAL_WINICON]->loadPalette(winiconPaletteDBG);
        break;
    }

    return new PaletteGroup(pal, PaletteGroup::PAL_NUM);
    ;
}

PaletteGroupFactory::PaletteGroupFactory()
{
}
