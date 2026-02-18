#ifndef FALLOUT2_TILE_HIRES_STENCIL_H
#define FALLOUT2_TILE_HIRES_STENCIL_H

#include "fallout2/geometry.h"

namespace Fallout2 {

void tile_hires_stencil_init();
void tile_hires_stencil_on_center_tile_or_elevation_change();
void tile_hires_stencil_draw(Rect *rect, unsigned char *buffer, int windowWidth, int windowHeight);

} // namespace Fallout2

#endif
