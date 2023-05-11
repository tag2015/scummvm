#include "fallout2/palette.h"

/// #include <string.h>

#include "common/debug.h"
#include "fallout2/color.h"
#include "fallout2/cycle.h"

// #include "debug.h"

// #include "game_sound.h" TODO
// #include "input.h" TODO
// #include "svga.h" TODO

namespace Fallout2 {

static void _palette_reset_();

// 0x6639D0
static unsigned char gPalette[256 * 3];

// 0x663CD0
unsigned char gPaletteWhite[256 * 3];

// 0x663FD0
unsigned char gPaletteBlack[256 * 3];

// 0x6642D0
static int gPaletteFadeSteps;

// 0x493A00
void paletteInit() {
	memset(gPaletteBlack, 0, 256 * 3);
	memset(gPaletteWhite, 63, 256 * 3);
	memcpy(gPalette, _cmap, 256 * 3);

	unsigned int tick = /* getTicks();*/ g_system->getMillis();
	// TODO audio
	//	if (backgroundSoundIsEnabled() || speechIsEnabled()) {
	//  colorPaletteSetTransitionCallback(soundContinueAll);
	//	}

	colorPaletteFadeBetween(gPalette, gPalette, 60);

	colorPaletteSetTransitionCallback(NULL);

	// Actual fade duration will never be 0 since |colorPaletteFadeBetween| uses
	// frame rate throttling.
	// TODO
	// unsigned int actualFadeDuration = getTicksSince(tick);
	unsigned int actualFadeDuration = getTicksBetween(g_system->getMillis(), tick);

	// Calculate fade steps needed to perform fading in about 700 ms.
	gPaletteFadeSteps = 60 * 700 / actualFadeDuration;

	debug("\nFade time is %u\nFade steps are %d", actualFadeDuration, gPaletteFadeSteps);
}

// NOTE: Collapsed.
//
// 0x493AD0
static void _palette_reset_() {
}

// NOTE: Uncollapsed 0x493AD0.
void paletteReset() {
	_palette_reset_();
}

// NOTE: Uncollapsed 0x493AD0.
void paletteExit() {
	_palette_reset_();
}

// 0x493AD4
void paletteFadeTo(unsigned char *palette) {
	bool colorCycleWasEnabled = colorCycleEnabled();
	colorCycleDisable();

	// TODO sound
	//	if (backgroundSoundIsEnabled() || speechIsEnabled()) {
	//		colorPaletteSetTransitionCallback(soundContinueAll);
	//	}

	colorPaletteFadeBetween(gPalette, palette, gPaletteFadeSteps);
	colorPaletteSetTransitionCallback(NULL);

	memcpy(gPalette, palette, 768);

	if (colorCycleWasEnabled) {
		colorCycleEnable();
	}
}

// 0x493B48
void paletteSetEntries(unsigned char *palette) {
	memcpy(gPalette, palette, sizeof(gPalette));
	_setSystemPalette(palette);
}

// 0x493B78
void paletteSetEntriesInRange(unsigned char *palette, int start, int end) {
	memcpy(gPalette + 3 * start, palette, 3 * (end - start + 1));
	_setSystemPaletteEntries(palette, start, end);
}

} // namespace Fallout2
