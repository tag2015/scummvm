#include "fallout2/mapper/mp_targt.h"

// #include <string.h>

#include "fallout2/art.h"
#include "fallout2/proto.h"
#include "fallout2/window_manager_private.h"

namespace Fallout2 {

// 0x53F354
static char default_target_path_base[] = "\\fallout2\\dev\\proto\\";

// 0x559CD0
static char *target_path_base = default_target_path_base;

// 0x559DBC
static bool tgt_overriden = false;

// 0x49B2F0
void target_override_protection() {
	// TODO: Incomplete.
}

// 0x49B2F0
bool target_overriden() {
	return tgt_overriden;
}

// 0x49B34C
void target_make_path(char *path, int pid) {
	if (_cd_path_base[0] != '\0' && _cd_path_base[1] == ':') {
		strncpy(path, _cd_path_base, 2);
		strcat(path, target_path_base);
	} else {
		strcpy(path, target_path_base);
	}

	if (pid != -1) {
		strcat(path, artGetObjectTypeName(PID_TYPE(pid)));
	}
}

// 0x49B424
int target_init() {
	// TODO: Incomplete.

	return 0;
}

// 0x49B434
int target_exit() {
	// TODO: Incomplete.

	return 0;
}

// 0x49BD98
int pick_rot() {
	int value;
	win_get_num_i(&value,
				  -1,
				  5,
				  false,
				  "Rotation",
				  100,
				  100);
	return value;
}

} // namespace Fallout2
