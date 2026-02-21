#ifndef FALLOUT2_REACTION_H
#define FALLOUT2_REACTION_H

#include "fallout2/obj_types.h"

namespace Fallout2 {

typedef enum NpcReaction {
	NPC_REACTION_BAD,
	NPC_REACTION_NEUTRAL,
	NPC_REACTION_GOOD,
} NpcReaction;

int reactionSetValue(Object *critter, int value);
int reactionTranslateValue(int value);
int _reaction_influence_();
int reactionGetValue(Object *critter);

} // namespace Fallout2

#endif
