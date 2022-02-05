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
#include "background.h"
#include "button.h"
#include "frame.h"
#include "progressbar.h"
#include "titlebar.h"
#include "window.h"
#include "window_icon.h"
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

//PALETTE_WINOS2 colors
/*
 * 0 - black    8 - gray
 * 1 - maroon   9 - red
 * 2 - green    10 - lime
 * 3 - olive    11 - yellow
 * 4 - navy     12 - blue
 * 5 - purple   13 - fuchsia
 * 6 - teal     14 - aqua
 * 7 - silver   15 - white
 */
static const unsigned PALETTEWINOS2[] = {BLACK, MAROON, GREEN, OLIVE, NAVY, PURPLE, TEAL, SILVER, GRAY, RED, LIME, YELLOW, BLUE, FUCHSIA, AQUA, WHITE};

//PALETTE_MACINTOSH
/*
 * 0 - white    8 - green
 * 1 - yellow   9 - dark green
 * 2 - orange   10 - brown
 * 3 - red      11 - tan
 * 4 - magenta  12 - light grey
 * 5 - purple   13 - medium grey
 * 6 - blue     14 - dark grey
 * 7 - cyan     15 - black
 */
static const unsigned PALETTEMAC[] = {WHITE, 0xFBF305, 0xFF6403, 0xDD0907, 0xF20884, 0x4700A5, 0x0000D3, 0x02ABEA, 0x1FB714, 0x006412, 0x562C05, 0x90713A, SILVER, GRAY, 0x404040, BLACK};

//PALETTE RISCOS
/*
 * 0 - white    8 - dark blue
 * 1 - grey #1  9 - yellow
 * 2 - grey #2  10 - green
 * 3 - grey #3  11 - red
 * 4 - grey #4  12 - beige
 * 5 - grey #5  13 - dark green
 * 6 - grey #6  14 - gold/orange
 * 7 - black    15 - light blue
 */
static const unsigned PALETTERISCOS[] = {WHITE, 0xDDDDDD, 0xBBBBBB, 0x999999, 0x777777, 0x555555, 0x333333, BLACK, 0x004499, 0xEEEE00, 0x00CC00, 0xDD0000, 0xEEEEBB, 0x558800, 0xFFBB00, 0x00BBFF};

// Palette set for  PALETTE_WINOS2
#define C(x) PALETTEWINOS2[x]

static const unsigned framePaletteWINOS2[] = {C(15), C(0), C(7), C(9), C(8), C(10)};       // Bright, dark, main, dragging, disabled, active
static const unsigned titlebarPaletteWINOS2[] = {C(12), C(15), C(8), C(0)};                // Background, text, background disabled, text disabled
static const unsigned backgroundPaletteWINOS2[] = {C(7), C(8)};                            // Background, disabled
static const unsigned progressbarPaletteWINOS2[] = {C(0), C(12), C(15), C(0), C(8), C(7)}; // Background, Foreground, text, bg disabled, fg disabled, text disabled
static const unsigned desktopPaletteWINOS2[] = {C(8)};                                     // Desktop background
static const unsigned buttonPaletteWINOS2[] = {C(15), C(0), C(7), C(9), C(0), C(11)};      // Bright, Dark, main, selected, disabled, pressed
static const unsigned winiconPaletteWINOS2[] = {C(15), C(0), C(7), C(9), C(0)};            // Bright, Dark, main, pressed, released

// Palette set for  PALETTE_DEBUG
static const unsigned framePaletteDBG[] = {RED, GREEN, BLUE};                          // Bright, dark, main
static const unsigned titlebarPaletteDBG[] = {YELLOW, WHITE};                          // Background, text
static const unsigned backgroundPaletteDBG[] = {AQUA};                                 // Background
static const unsigned progressbarPaletteDBG[] = {BLACK, BLUE, WHITE};                  // Background, Foreground, text
static const unsigned desktopPaletteDBG[] = {BLACK};                                   // Desktop background
static const unsigned buttonPaletteDBG[] = {WHITE, BLACK, SILVER, RED, BLACK, YELLOW}; // Bright, Dark, main, selected, disabled, pressed
static const unsigned winiconPaletteDBG[] = {WHITE, BLACK, SILVER, RED, BLACK};        // Bright, Dark, main, pressed, released

// Palette set for  PALETTE_MACINTOSH
#undef C
#define C(x) PALETTEMAC[x]

static const unsigned framePaletteMAC[] = {C(12), C(14), C(13)};                       // Bright, dark, main
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

    pal[PaletteGroup::PAL_BACKGROUND] = new PaletteTrueColor(BACKGROUND_PAL_NUM);
    pal[PaletteGroup::PAL_FRAME] = new PaletteTrueColor(FRAME_PAL_NUM);
    pal[PaletteGroup::PAL_TITLEBAR] = new PaletteTrueColor(TITLEBAR_PAL_NUM);
    pal[PaletteGroup::PAL_PROGRESSBAR] = new PaletteTrueColor(PROGRESSBAR_PAL_NUM);
    // FIXME: where is this one ?
    pal[PaletteGroup::PAL_DESKTOP] = new PaletteTrueColor(1);
    pal[PaletteGroup::PAL_BUTTON] = new PaletteTrueColor(BUTTON_PAL_NUM);
    pal[PaletteGroup::PAL_WINICON] = new PaletteTrueColor(WINICON_PAL_NUM);

    switch (sel)
    {
    case PALETTE_WINOS2:
        pal[PaletteGroup::PAL_BACKGROUND]->loadPalette(backgroundPaletteWINOS2);
        pal[PaletteGroup::PAL_FRAME]->loadPalette(framePaletteWINOS2);
        pal[PaletteGroup::PAL_TITLEBAR]->loadPalette(titlebarPaletteWINOS2);
        pal[PaletteGroup::PAL_PROGRESSBAR]->loadPalette(progressbarPaletteWINOS2);
        pal[PaletteGroup::PAL_DESKTOP]->loadPalette(desktopPaletteWINOS2);
        pal[PaletteGroup::PAL_BUTTON]->loadPalette(buttonPaletteWINOS2);
        pal[PaletteGroup::PAL_WINICON]->loadPalette(winiconPaletteWINOS2);
        break;
    case PALETTE_MACINTOSH:
        pal[PaletteGroup::PAL_BACKGROUND]->loadPalette(backgroundPaletteMAC);
        pal[PaletteGroup::PAL_FRAME]->loadPalette(framePaletteMAC);
        pal[PaletteGroup::PAL_TITLEBAR]->loadPalette(titlebarPaletteMAC);
        pal[PaletteGroup::PAL_PROGRESSBAR]->loadPalette(progressbarPaletteMAC);
        pal[PaletteGroup::PAL_DESKTOP]->loadPalette(desktopPaletteMAC);
        pal[PaletteGroup::PAL_BUTTON]->loadPalette(buttonPaletteMAC);
        pal[PaletteGroup::PAL_WINICON]->loadPalette(winiconPaletteMAC);
        break;
    case PALETTE_RISCOS:
        return nullptr;

    case PALETTE_DEBUG:
        pal[PaletteGroup::PAL_BACKGROUND]->loadPalette(backgroundPaletteDBG);
        pal[PaletteGroup::PAL_FRAME]->loadPalette(framePaletteDBG);
        pal[PaletteGroup::PAL_TITLEBAR]->loadPalette(titlebarPaletteDBG);
        pal[PaletteGroup::PAL_PROGRESSBAR]->loadPalette(progressbarPaletteDBG);
        pal[PaletteGroup::PAL_DESKTOP]->loadPalette(desktopPaletteDBG);
        pal[PaletteGroup::PAL_BUTTON]->loadPalette(buttonPaletteDBG);
        pal[PaletteGroup::PAL_WINICON]->loadPalette(winiconPaletteDBG);
        break;
    }

    return new PaletteGroup(pal, PaletteGroup::PAL_NUM);
    ;
}

PaletteGroupFactory::PaletteGroupFactory()
{
}
