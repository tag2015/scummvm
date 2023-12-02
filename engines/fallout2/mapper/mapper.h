#ifndef FALLOUT2_MAPPER_H
#define FALLOUT2_MAPPER_H

#include "fallout2/map.h"
#include "fallout2/obj_types.h"

namespace Fallout2 {

extern MapTransition mapInfo;

int mapper_inven_unwield(Object *obj, int right_hand);

} // namespace Fallout2

#endif
