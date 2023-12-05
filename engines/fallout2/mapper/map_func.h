#ifndef FALLOUT2_MAPPER_MAP_FUNC_H
#define FALLOUT2_MAPPER_MAP_FUNC_H

#include "fallout2/geometry.h"

namespace Fallout2 {

void setup_map_dirs();
void copy_proto_lists();
void sort_rect(Rect *a, Rect *b);
void draw_rect(Rect *rect);
void erase_rect(Rect *rect);
int toolbar_proto(int type, int id);
bool map_toggle_block_obj_viewing_on();

} // namespace Fallout2

#endif
