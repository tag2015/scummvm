#include "fallout2/mapper/mp_proto.h"

// #include <string.h>

#include "fallout2/color.h"
#include "fallout2/combat_ai.h"
#include "fallout2/critter.h"
#include "fallout2/memory.h"
#include "fallout2/proto.h"
#include "fallout2/window_manager_private.h"

namespace Fallout2 {

#define CRITTER_FLAG_COUNT 10

static int proto_critter_flags_modify(int pid);
static int mp_pick_kill_type();

// 0x559C60
bool can_modify_protos = false;

// 0x559C6C
static int critFlagList[CRITTER_FLAG_COUNT] = {
	CRITTER_NO_STEAL,
	CRITTER_NO_DROP,
	CRITTER_NO_LIMBS,
	CRITTER_NO_AGE,
	CRITTER_NO_HEAL,
	CRITTER_INVULNERABLE,
	CRITTER_FLAT,
	CRITTER_SPECIAL_DEATH,
	CRITTER_LONG_LIMBS,
	CRITTER_NO_KNOCKBACK,
};

// 0x4922F8
void init_mapper_protos() {
	// TODO: Incomplete.
}

// 0x496120
int proto_critter_flags_modify(int pid) {
	Proto *proto;
	int rc;
	int flags = 0;
	int index;

	if (protoGetProto(pid, &proto) == -1) {
		return -1;
	}

	rc = win_yes_no("Can't be stolen from?", 340, 200, _colorTable[15855]);
	if (rc == -1) {
		return -1;
	}

	if (rc == 1) {
		flags |= CRITTER_NO_STEAL;
	}

	rc = win_yes_no("Can't Drop items?", 340, 200, _colorTable[15855]);
	if (rc == -1) {
		return -1;
	}

	if (rc == 1) {
		flags |= CRITTER_NO_DROP;
	}

	rc = win_yes_no("Can't lose limbs?", 340, 200, _colorTable[15855]);
	if (rc == -1) {
		return -1;
	}

	if (rc == 1) {
		flags |= CRITTER_NO_LIMBS;
	}

	rc = win_yes_no("Dead Bodies Can't Age?", 340, 200, _colorTable[15855]);
	if (rc == -1) {
		return -1;
	}

	if (rc == 1) {
		flags |= CRITTER_NO_AGE;
	}

	rc = win_yes_no("Can't Heal by Aging?", 340, 200, _colorTable[15855]);
	if (rc == -1) {
		return -1;
	}

	if (rc == 1) {
		flags |= CRITTER_NO_HEAL;
	}

	rc = win_yes_no("Is Invlunerable????", 340, 200, _colorTable[15855]);
	if (rc == -1) {
		return -1;
	}

	if (rc == 1) {
		flags |= CRITTER_INVULNERABLE;
	}

	rc = win_yes_no("Can't Flatten on Death?", 340, 200, _colorTable[15855]);
	if (rc == -1) {
		return -1;
	}

	if (rc == 1) {
		flags |= CRITTER_FLAT;
	}

	rc = win_yes_no("Has Special Death?", 340, 200, _colorTable[15855]);
	if (rc == -1) {
		return -1;
	}

	if (rc == 1) {
		flags |= CRITTER_SPECIAL_DEATH;
	}

	rc = win_yes_no("Has Extra Hand-To-Hand Range?", 340, 200, _colorTable[15855]);
	if (rc == -1) {
		return -1;
	}

	if (rc == 1) {
		flags |= CRITTER_LONG_LIMBS;
	}

	rc = win_yes_no("Can't be knocked back?", 340, 200, _colorTable[15855]);
	if (rc == -1) {
		return -1;
	}

	if (rc == 1) {
		flags |= CRITTER_NO_KNOCKBACK;
	}

	if (!can_modify_protos) {
		win_timed_msg("Can't modify protos!", _colorTable[31744] | 0x10000);
		return -1;
	}

	for (index = 0; index < CRITTER_FLAG_COUNT; index++) {
		if ((critFlagList[index] & flags) != 0) {
			critter_flag_set(pid, critFlagList[index]);
		} else {
			critter_flag_unset(pid, critFlagList[index]);
		}
	}

	return 0;
}

// 0x497520
int mp_pick_kill_type() {
	char *names[KILL_TYPE_COUNT];
	int index;

	for (index = 0; index < KILL_TYPE_COUNT; index++) {
		names[index] = killTypeGetName(index);
	}

	return _win_list_select("Kill Type",
							names,
							KILL_TYPE_COUNT,
							NULL,
							50,
							100,
							_colorTable[15855]);
}

// 0x497568
int proto_pick_ai_packet(int *value) {
	int count;
	char **names;
	int index;
	int rc;

	count = combat_ai_num();
	if (count <= 0) {
		return -1;
	}

	names = (char **)internal_malloc(sizeof(char *) * count);
	for (index = 0; index < count; index++) {
		names[index] = (char *)internal_malloc(strlen(combat_ai_name(index)) + 1);
		strcpy(names[index], combat_ai_name(index));
	}

	rc = _win_list_select("AI Packet",
						  names,
						  count,
						  NULL,
						  50,
						  100,
						  _colorTable[15855]);
	if (rc != -1) {
		*value = rc;
	}

	for (index = 0; index < count; index++) {
		internal_free(names[index]);
	}

	internal_free(names);
	return 0;
}

} // namespace Fallout2
