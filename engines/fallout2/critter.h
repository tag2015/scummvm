#ifndef FALLOUT2_CRITTER_H
#define FALLOUT2_CRITTER_H

#include "fallout2/db.h"
#include "fallout2/obj_types.h"
#include "fallout2/proto_types.h"

#include "common/savefile.h"

namespace Fallout2 {

typedef enum DudeState {
	DUDE_STATE_SNEAKING = 0,
	DUDE_STATE_LEVEL_UP_AVAILABLE = 3,
	DUDE_STATE_ADDICTED = 4,
} DudeState;

int critterInit();
void critterReset();
void critterExit();
int critterLoad(File *stream);
int critterSave(Common::OutSaveFile *stream);
char *critterGetName(Object *obj);
void critterProtoDataCopy(CritterProtoData *dest, CritterProtoData *src);
int dudeSetName(const char *name);
void dudeResetName();
int critterGetHitPoints(Object *critter);
int critterAdjustHitPoints(Object *critter, int hp);
int critterGetPoison(Object *critter);
int critterAdjustPoison(Object *obj, int amount);
int poisonEventProcess(Object *obj, void *data);
int critterGetRadiation(Object *critter);
int critterAdjustRadiation(Object *obj, int amount);
int _critter_check_rads(Object *critter);
int _clear_rad_damage(Object *obj, void *data);
void _process_rads(Object *obj, int radiationLevel, bool direction);
int radiationEventProcess(Object *obj, void *data);
int radiationEventRead(Common::InSaveFile *stream, void **dataPtr);
int radiationEventWrite(Common::OutSaveFile *stream, void *data);
int critterGetDamageType(Object *critter);
int killsIncByType(int killType);
int killsGetByType(int killType);
int killsLoad(Common::InSaveFile *stream);
int killsSave(Common::OutSaveFile *stream);
int critterGetKillType(Object *critter);
char *killTypeGetName(int killType);
char *killTypeGetDescription(int killType);
int _critter_heal_hours(Object *obj, int a2);
void critterKill(Object *critter, int anim, bool a3);
int critterGetExp(Object *critter);
bool critterIsActive(Object *critter);
bool critterIsDead(Object *critter);
bool critterIsCrippled(Object *critter);
bool _critter_is_prone(Object *critter);
int critterGetBodyType(Object *critter);
int gcdLoad(const char *path);
int gcdLoadScumm(const char *path);
int protoCritterDataRead(File *stream, CritterProtoData *critterData);
int gcdSave(const char *path);
int protoCritterDataWrite(Common::WriteStream *stream, CritterProtoData *critterData);
void dudeDisableState(int state);
void dudeEnableState(int state);
void dudeToggleState(int state);
bool dudeHasState(int state);
int sneakEventProcess(Object *obj, void *data);
int _critter_sneak_clear(Object *obj, void *data);
bool dudeIsSneaking();
int knockoutEventProcess(Object *obj, void *data);
int _critter_wake_clear(Object *obj, void *data);
int _critter_set_who_hit_me(Object *a1, Object *a2);
bool _critter_can_obj_dude_rest();
int critterGetMovementPointCostAdjustedForCrippledLegs(Object *critter, int a2);
bool critterIsEncumbered(Object *critter);
bool critterIsFleeing(Object *a1);
bool _critter_flag_check(int pid, int flag);

} // namespace Fallout2

#endif
