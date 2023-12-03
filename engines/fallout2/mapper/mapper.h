#ifndef FALLOUT2_MAPPER_H
#define FALLOUT2_MAPPER_H

#include "fallout2/map.h"
#include "fallout2/obj_types.h"

namespace Fallout2 {

extern MapTransition mapInfo;

extern int menu_val_0[8];
extern int menu_val_2[8];
extern int menu_val_1[21];
extern unsigned char *tool;
extern int tool_win;

void print_toolbar_name(int object_type);
int mapper_inven_unwield(Object *obj, int right_hand);

} // namespace Fallout2

#endif
