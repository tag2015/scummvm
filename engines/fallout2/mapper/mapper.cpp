#include "fallout2/mapper/mapper.h"

#include "fallout2/animation.h"
#include "fallout2/art.h"
#include "fallout2/game_mouse.h"
#include "fallout2/inventory.h"
#include "fallout2/object.h"
#include "fallout2/proto.h"

namespace Fallout2 {

static int mapper_mark_exit_grid();
static void mapper_mark_all_exit_grids();

// 0x559748
MapTransition mapInfo = {-1, -1, 0, 0};

// 0x48C604
int mapper_inven_unwield(Object *obj, int right_hand) {
	Object *item;
	int fid;

	reg_anim_begin(ANIMATION_REQUEST_RESERVED);

	if (right_hand) {
		item = critterGetItem2(obj);
	} else {
		item = critterGetItem1(obj);
	}

	if (item != NULL) {
		item->flags &= ~OBJECT_IN_ANY_HAND;
	}

	animationRegisterAnimate(obj, ANIM_PUT_AWAY, 0);

	fid = buildFid(OBJ_TYPE_CRITTER, obj->fid & 0xFFF, 0, 0, (obj->fid & 0x70000000) >> 28);
	animationRegisterSetFid(obj, fid, 0);

	return reg_anim_end();
}

// 0x48C678
int mapper_mark_exit_grid() {
	int y;
	int x;
	int tile;
	Object *obj;

	for (y = -2000; y != 2000; y += 200) {
		for (x = -10; x < 10; x++) {
			tile = gGameMouseBouncingCursor->tile + y + x;

			obj = objectFindFirstAtElevation(gElevation);
			while (obj != NULL) {
				if (isExitGridPid(obj->pid)) {
					obj->data.misc.map = mapInfo.map;
					obj->data.misc.tile = mapInfo.tile;
					obj->data.misc.elevation = mapInfo.elevation;
					obj->data.misc.rotation = mapInfo.rotation;
				}
				obj = objectFindNextAtElevation();
			}
		}
	}

	return 0;
}

// 0x48C704
void mapper_mark_all_exit_grids() {
	Object *obj;

	obj = objectFindFirstAtElevation(gElevation);
	while (obj != NULL) {
		if (isExitGridPid(obj->pid)) {
			obj->data.misc.map = mapInfo.map;
			obj->data.misc.tile = mapInfo.tile;
			obj->data.misc.elevation = mapInfo.elevation;
			obj->data.misc.rotation = mapInfo.rotation;
		}
		obj = objectFindNextAtElevation();
	}
}

} // namespace Fallout2