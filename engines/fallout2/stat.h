#ifndef FALLOUT2_STAT_H
#define FALLOUT2_STAT_H

#include "fallout2/db.h"
#include "fallout2/obj_types.h"
#include "fallout2/proto_types.h"
#include "fallout2/stat_defs.h"

#include "common/savefile.h"

namespace Fallout2 {

#define STAT_ERR_INVALID_STAT (-5)

int statsInit();
int statsReset();
int statsExit();
int statsLoad(Common::InSaveFile *stream);
int statsSave(Common::OutSaveFile *stream);
int critterGetStat(Object *critter, int stat);
int critterGetBaseStatWithTraitModifier(Object *critter, int stat);
int critterGetBaseStat(Object *critter, int stat);
int critterGetBonusStat(Object *critter, int stat);
int critterSetBaseStat(Object *critter, int stat, int value);
int critterIncBaseStat(Object *critter, int stat);
int critterDecBaseStat(Object *critter, int stat);
int critterSetBonusStat(Object *critter, int stat, int value);
void protoCritterDataResetStats(CritterProtoData *data);
void critterUpdateDerivedStats(Object *critter);
char *statGetName(int stat);
char *statGetDescription(int stat);
char *statGetValueDescription(int value);
int pcGetStat(int pcStat);
int pcSetStat(int pcStat, int value);
void pcStatsReset();
int pcGetExperienceForNextLevel();
int pcGetExperienceForLevel(int level);
char *pcStatGetName(int pcStat);
char *pcStatGetDescription(int pcStat);
int statGetFrmId(int stat);
int statRoll(Object *critter, int stat, int modifier, int *howMuch);
int pcAddExperience(int xp, int *xpGained = nullptr);
int pcAddExperienceWithOptions(int xp, bool a2, int *xpGained = nullptr);
int pcSetExperience(int a1);

static inline bool statIsValid(int stat) {
	return stat >= 0 && stat < STAT_COUNT;
}

static inline bool pcStatIsValid(int pcStat) {
	return pcStat >= 0 && pcStat < PC_STAT_COUNT;
}

} // namespace Fallout2

#endif
