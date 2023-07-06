#ifndef _COLOR_UTILS_H_
#define _COLOR_UTILS_H_

#include <cstdint>

struct ARGB
{
	uint8_t a, r, g, b;
};

union ARGBColor
{
	struct ARGB colorARGB;
	uint8_t colorAR[8];
	uint32_t color;
};

// TODO: this should take into account little and big endian
inline void toARGBColor(const uint32_t ARGB, union ARGBColor *clr)
{
	clr->colorARGB.b = ARGB & 0xFF;
	clr->colorARGB.g = (ARGB >> 8) & 0xFF;
	clr->colorARGB.r = (ARGB >> 16) & 0xFF;
	clr->colorARGB.a = (ARGB >> 24) & 0xFF;
}

#endif