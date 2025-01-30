#include "fallout2/mapper/mp_scrpt.h"

#include "fallout2/art.h"
#include "fallout2/object.h"
#include "fallout2/scripts.h"
#include "fallout2/tile.h"

namespace Fallout2 {

// 0x49B214
int map_scr_remove_all_spatials() {
	int elevation;
	Script *scr;
	Object *obj;
	int sid;

	for (elevation = 0; elevation < ELEVATION_COUNT; elevation++) {
		scr = scriptGetFirstSpatialScript(elevation);
		while (scr != NULL) {
			scriptRemove(scr->sid);

			scr = scriptGetFirstSpatialScript(elevation);
		}

		obj = objectFindFirstAtElevation(elevation);
		while (obj != NULL) {
			if (buildFid(OBJ_TYPE_INTERFACE, 3, 0, 0, 0) == obj->fid) {
				objectDestroy(obj, NULL);

				obj = objectFindFirstAtElevation(elevation);
				continue;
			}

			obj = objectFindNextAtElevation();
		}
	}

	tileWindowRefresh();

	for (sid = 0; sid < 15000; sid++) {
		if (scriptGetScript(sid, &scr) != -1) {
			if (scr->owner != NULL) {
				if (scr->owner->pid == 0x500000C) {
					scr->owner->sid = -1;
					scriptRemove(sid);
				}
			}
		}
	}

	return 0;
}

} // namespace Fallout2
