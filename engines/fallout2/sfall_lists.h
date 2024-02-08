#ifndef FALLOUT2_SFALL_LISTS_H
#define FALLOUT2_SFALL_LISTS_H

// #include <vector>

#include "fallout2/obj_types.h"

#include "common/array.h"

namespace Fallout2 {

enum ListType {
	LIST_CRITTERS = 0,
	LIST_ITEMS = 1,
	LIST_SCENERY = 2,
	LIST_WALLS = 3,
	LIST_TILES = 4,
	LIST_MISC = 5,
	LIST_SPATIAL = 6,
	LIST_ALL = 9,
};

bool sfallListsInit();
void sfallListsReset();
void sfallListsExit();
int sfallListsCreate(int listType);
Object *sfallListsGetNext(int listId);
void sfallListsDestroy(int listId);
void sfall_lists_fill(int type, Common::Array<Object *> &objects); // std::vector

} // namespace Fallout2

#endif
