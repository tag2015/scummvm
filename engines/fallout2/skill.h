#ifndef FALLOUT2_SKILL_H
#define FALLOUT2_SKILL_H

#include "fallout2/db.h"
#include "fallout2/obj_types.h"
#include "fallout2/proto_types.h"
#include "fallout2/skill_defs.h"

#include "common/savefile.h"

namespace Fallout2 {

extern int _gIsSteal;
extern int _gStealCount;
extern int _gStealSize;

int skillsInit();
void skillsReset();
void skillsExit();
int skillsLoad(File *stream);
int skillsLoadScumm(Common::InSaveFile *stream);
int skillsSave(Common::OutSaveFile *stream);
void protoCritterDataResetSkills(CritterProtoData *data);
void skillsSetTagged(int *skills, int count);
void skillsGetTagged(int *skills, int count);
bool skillIsTagged(int skill);
int skillGetValue(Object *critter, int skill);
int skillGetDefaultValue(int skill);
int skillGetBaseValue(Object *critter, int skill);
int skillAdd(Object *critter, int skill);
int skillAddForce(Object *critter, int skill);
int skillsGetCost(int a1);
int skillSub(Object *critter, int skill);
int skillSubForce(Object *critter, int skill);
int skillRoll(Object *critter, int skill, int a3, int *a4);
char *skillGetName(int skill);
char *skillGetDescription(int skill);
char *skillGetAttributes(int skill);
int skillGetFrmId(int skill);
int skillUse(Object *obj, Object *a2, int skill, int a4);
int skillsPerformStealing(Object *a1, Object *a2, Object *item, bool isPlanting);
int skillGetGameDifficultyModifier(int skill);
int skillUpdateLastUse(int skill);
int skillsUsageSave(Common::OutSaveFile *stream);
int skillsUsageLoad(File *stream);
char *skillsGetGenericResponse(Object *critter, bool isDude);

// Returns true if skill is valid.
static inline bool skillIsValid(int skill) {
	return skill >= 0 && skill < SKILL_COUNT;
}

} // namespace Fallout2

#endif
