#ifndef FALLOUT2_ACTIONS_H
#define FALLOUT2_ACTIONS_H

#include "fallout2/combat_defs.h"
#include "fallout2/obj_types.h"

namespace Fallout2 {

extern int rotation;

int _action_attack(Attack *attack);
int _action_use_an_item_on_object(Object *a1, Object *a2, Object *a3);
int _action_use_an_object(Object *a1, Object *a2);
int actionPickUp(Object *critter, Object *item);
int _action_loot_container(Object *critter, Object *container);
int _action_skill_use(int a1);
int actionUseSkill(Object *a1, Object *a2, int skill);
bool _is_hit_from_front(Object *attacker, Object *defender);
bool _can_see(Object *a1, Object *a2);
bool _action_explode_running();
int actionExplode(int tile, int elevation, int minDamage, int maxDamage, Object *sourceObj, bool animate);
int actionTalk(Object *a1, Object *a2);
void actionDamage(int tile, int elevation, int minDamage, int maxDamage, int damageType, bool animated, bool bypassArmor);
bool actionCheckPush(Object *a1, Object *a2);
int actionPush(Object *a1, Object *a2);
int _action_can_talk_to(Object *a1, Object *a2);

} // namespace Fallout2

#endif
