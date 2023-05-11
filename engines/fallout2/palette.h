#ifndef FALLOUT2_PALETTE_H
#define FALLOUT2_PALETTE_H

namespace Fallout2 {

extern unsigned char gPaletteWhite[256 * 3];
extern unsigned char gPaletteBlack[256 * 3];

void paletteInit();
void paletteReset();
void paletteExit();
void paletteFadeTo(unsigned char *palette);
void paletteSetEntries(unsigned char *palette);
void paletteSetEntriesInRange(unsigned char *palette, int start, int end);

} // namespace Fallout2

#endif
